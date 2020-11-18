namespace filesync
{
    class tcp_client
    {
    private:
        const char *ip;

    public:
        tcp_client(char *ip);
        ~tcp_client();
        int connect() const;
    };
} // namespace filesync