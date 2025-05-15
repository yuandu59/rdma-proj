// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rdma/rdma_cma.h>

#define PORT "7471"
#define BUF_SIZE 1024

int main() {
    struct rdma_cm_id *listen_id = NULL, *conn_id = NULL;
    struct rdma_event_channel *ec = NULL;
    struct rdma_cm_event *event = NULL;
    struct ibv_pd *pd = NULL;
    struct ibv_mr *mr = NULL;
    char *recv_buf = NULL;
    struct ibv_comp_channel *comp_chan;
    struct ibv_cq *cq;
    struct ibv_qp_init_attr qp_attr = {0};
    struct ibv_recv_wr recv_wr = {0}, *bad_recv_wr = NULL;
    struct ibv_sge sge;
    struct ibv_wc wc;

    // 创建 event channel 和 listen ID
    ec = rdma_create_event_channel();
    rdma_create_id(ec, &listen_id, NULL, RDMA_PS_TCP);

    // 绑定 IP 和端口
    struct addrinfo hints = {
        .ai_flags = AI_PASSIVE,
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM
    }, *res;
    getaddrinfo(NULL, PORT, &hints, &res);
    rdma_bind_addr(listen_id, res->ai_addr);
    freeaddrinfo(res);

    // 开始监听
    rdma_listen(listen_id, 1);
    printf("Server is listening on port %s...\n", PORT);

    // 等待连接请求
    rdma_get_cm_event(ec, &event);
    if (event->event != RDMA_CM_EVENT_CONNECT_REQUEST) {
        fprintf(stderr, "Unexpected event\n");
        return 1;
    }
    conn_id = event->id;
    rdma_ack_cm_event(event);

    // 分配资源
    pd = ibv_alloc_pd(conn_id->verbs);
    recv_buf = malloc(BUF_SIZE);
    mr = ibv_reg_mr(pd, recv_buf, BUF_SIZE, IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE);

    // 创建QP
    qp_attr.cap.max_send_wr = 1;
    qp_attr.cap.max_recv_wr = 1;
    qp_attr.cap.max_send_sge = 1;
    qp_attr.cap.max_recv_sge = 1;
    qp_attr.qp_type = IBV_QPT_RC;
    qp_attr.send_cq = cq = ibv_create_cq(conn_id->verbs, 2, NULL, NULL, 0);
    qp_attr.recv_cq = cq;
    rdma_create_qp(conn_id, pd, &qp_attr);

    // Post a receive
    sge.addr = (uintptr_t)recv_buf;
    sge.length = BUF_SIZE;
    sge.lkey = mr->lkey;
    recv_wr.sg_list = &sge;
    recv_wr.num_sge = 1;
    ibv_post_recv(conn_id->qp, &recv_wr, &bad_recv_wr);

    // 接受连接
    struct rdma_conn_param conn_param = {0};
    rdma_accept(conn_id, &conn_param);

    // 等待接收完成
    while (ibv_poll_cq(cq, 1, &wc) == 0) {}
    printf("Received message: %s\n", recv_buf);

    // 清理资源
    rdma_disconnect(conn_id);
    rdma_destroy_qp(conn_id);
    ibv_dereg_mr(mr);
    free(recv_buf);
    ibv_destroy_cq(cq);
    ibv_dealloc_pd(pd);
    rdma_destroy_id(conn_id);
    rdma_destroy_id(listen_id);
    rdma_destroy_event_channel(ec);
    return 0;
}
