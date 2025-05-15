// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rdma/rdma_cma.h>

#define PORT "7471"
#define BUF_SIZE 1024

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <server-ip>\n", argv[0]);
        return 1;
    }

    struct rdma_cm_id *conn_id = NULL;
    struct rdma_event_channel *ec = NULL;
    struct rdma_cm_event *event = NULL;
    struct ibv_pd *pd;
    struct ibv_mr *mr;
    char *send_buf;
    struct ibv_qp_init_attr qp_attr = {0};
    struct ibv_sge sge;
    struct ibv_send_wr send_wr = {0}, *bad_send_wr = NULL;
    struct ibv_cq *cq;

    // 创建 ID 和事件通道
    ec = rdma_create_event_channel();
    rdma_create_id(ec, &conn_id, NULL, RDMA_PS_TCP);

    // 解析地址并连接
    struct addrinfo *res;
    getaddrinfo(argv[1], PORT, NULL, &res);
    rdma_resolve_addr(conn_id, NULL, res->ai_addr, 2000);
    rdma_get_cm_event(ec, &event);
    rdma_ack_cm_event(event);
    rdma_resolve_route(conn_id, 2000);
    rdma_get_cm_event(ec, &event);
    rdma_ack_cm_event(event);

    // 分配资源
    pd = ibv_alloc_pd(conn_id->verbs);
    send_buf = strdup("Hello RDMA!");
    mr = ibv_reg_mr(pd, send_buf, BUF_SIZE, IBV_ACCESS_LOCAL_WRITE);

    // 创建QP
    qp_attr.cap.max_send_wr = 1;
    qp_attr.cap.max_recv_wr = 1;
    qp_attr.cap.max_send_sge = 1;
    qp_attr.cap.max_recv_sge = 1;
    qp_attr.qp_type = IBV_QPT_RC;
    qp_attr.send_cq = cq = ibv_create_cq(conn_id->verbs, 2, NULL, NULL, 0);
    qp_attr.recv_cq = cq;
    rdma_create_qp(conn_id, pd, &qp_attr);

    // 发起连接
    struct rdma_conn_param conn_param = {0};
    rdma_connect(conn_id, &conn_param);
    rdma_get_cm_event(ec, &event);
    rdma_ack_cm_event(event);

    // 发送消息
    sge.addr = (uintptr_t)send_buf;
    sge.length = strlen(send_buf) + 1;
    sge.lkey = mr->lkey;
    send_wr.sg_list = &sge;
    send_wr.num_sge = 1;
    send_wr.opcode = IBV_WR_SEND;
    send_wr.send_flags = IBV_SEND_SIGNALED;
    ibv_post_send(conn_id->qp, &send_wr, &bad_send_wr);

    printf("Sent: %s\n", send_buf);

    // 清理资源
    sleep(1);
    rdma_disconnect(conn_id);
    rdma_destroy_qp(conn_id);
    ibv_dereg_mr(mr);
    free(send_buf);
    ibv_destroy_cq(cq);
    ibv_dealloc_pd(pd);
    rdma_destroy_id(conn_id);
    rdma_destroy_event_channel(ec);
    return 0;
}
