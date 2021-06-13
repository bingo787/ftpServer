cmake_minimum_required(VERSION 3.13)
project(ftpserver)

add_executable(ftpserver
        FtpCommandMap.c
        FtpProto.c
        Main.c
        FtpPrivOpCommand.c
        FtpSession.c
        FtpCommonHeader.c
        FtpTransportCtrl.c
        FtpGlobalConfiguration.c
        FtpNobodyUser.c
        FtpCryptoHash.c
        FtpParseConfig.c
        FtpPrivSock.c
        FtpTransportData.c)
target_link_libraries(ftpserver crypt)
