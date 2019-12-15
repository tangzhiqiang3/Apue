
void set_socket_rlimit(void)
{
    struct rlimit  limit ={0};

    getrlimit(RLIMIT_NOFIFE, &limit);
    limit.rlim_cur = limit.rlim_max;
    setrlimit(RLIMIT_NOFILE, &limit );

    printf("set socket open fd max count to %d\n", limit.rlim_max);
}