cmake_minimum_required(VERSION 3.13)
project(ftpserver)

add_executable(ftpserver
        command_map.c
        ftp_assist.c
        ftp_proto.c
        main.c
        priv_command.c
        session.c
        utils.c
        transport_ctrl.c
        configuration.c
        ftp_nobody.c
        hash.c
        parse_conf.c
        priv_sock.c
        transport_data.c)
target_link_libraries(ftpserver crypt)
