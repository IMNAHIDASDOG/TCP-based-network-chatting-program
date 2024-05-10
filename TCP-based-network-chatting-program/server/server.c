#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> 
#include<conio.h>
#include<string.h>
#include<winsock.h> 
#include<time.h>

/*����������ͷ�ļ�*/
int main()
{
    char Sendbuf[100];                          /*�������ݵĻ�����*/
    char Receivebuf[100];                       /*�������ݵĻ�����*/
    int SendLen;                                /*�������ݵĳ���*/
    int ReceiveLen;                             /*�������ݵĳ���*/
    int Length;                                 /*��ʾSOCKADDR�Ĵ�С*/
    unsigned short int Point;                   /*�˿ں�*/
    int Check1;                                 /*����ʧ���������*/
    char END[10] = "#END\0";                    /*�洢������־�ַ�*/

    SOCKET socket_server;                       /*�����׽���*/
    SOCKET socket_receive;                      /*���������׽���*/

    SOCKADDR_IN Server_add;                     /*��������ַ��Ϣ�ṹ*/
    SOCKADDR_IN Client_add;                     /*�ͻ�����Ϣ��ַ�ṹ*/

    WORD wVersionRequested;                     /*�֣�word����unsigned short*/
    WSADATA wsaData;                            /*��汾��Ϣ�ṹ*/
    int error;                                  /*��ʾ����*/

    FILE* fp;                                   /*�ļ�ָ��*/
    struct tm* sysTime;                         /*ϵͳʱ��ָ��*/

    /*ָ���˿�*/
t1: printf("��ָ���˿ں�:");
    scanf("%hu", &Point);

    /*---------------------------��ʼ���׽��ֿ�--------------------------------*/

    /*����汾���͡��������ֽ���ϳ�һ���֣�ǰ���ǵ��ֽڣ������Ǹ��ֽ�*/
    wVersionRequested = MAKEWORD(2, 2);             /*�����׽��ֿ⣬��ʼ�� Ws232.dll��̬���ӿ�*/
    error = WSAStartup(wVersionRequested, &wsaData);
    if (error != 0)
    {
        printf("�����׽���ʧ�ܣ�\n");
        printf("�����˳��������������\n");
        _getch();
        return 0;                                    /*�����������*/
    }
    /*�ж�������صİ汾���Ƿ����Ҫ��*/
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        printf("�����׽���ʧ�ܣ�\n");
        WSACleanup();                               /*�����ϣ��ر��׽��ֿ�*/
        printf("�����˳��������������\n");
        _getch();
        return 0;                                   /*��������˳�*/
    }

    /*---------------------------------�������ӵ�ַ----------------------------------*/

    Server_add.sin_family = AF_INET;                             /*��ַ���壬������AF_INET,ע��ֻ�������������ֽ�˳��*/
    Server_add.sin_addr.S_un.S_addr = htonl(INADDR_ANY);         /*������ַ*/
    Server_add.sin_port = htons(Point);                          /*�˿ں�*/
    /*AF_INET��ʾָ����ַ�壬SOCK_STREAM��ʾ��ʽ�׽���TCP���ض��ĵ�ַ������ص�Э��*/
    socket_server = socket(AF_INET, SOCK_STREAM, 0);
    /*---���׽��ֵ����ص�ĳ����ַ�Ͷ˿���---*/
    /*socket_ serverΪ�׽��֣�(SOCKADDR*)&Server_addΪ��������ַ*/
    if (bind(socket_server, (SOCKADDR*)&Server_add, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        printf("��ʧ��\n");
        printf("�����˳��������������\n");
        _getch();                                       
        closesocket(socket_server);
        WSACleanup();
        return 0;                                                 /*�����������*/
    }

    /*----------------------------�����׽���Ϊ����״̬------------------------------*/

    /*����״̬��Ϊ������׼�������ȴ�����ĿΪ5*/
    if (listen(socket_server, 5) < 0)
    {
        printf("����ʧ��\n");
        printf("�����˳��������������\n");
        _getch();
        closesocket(socket_server);
        WSACleanup();
        return 0;                                                 /*�����������*/
    }

    /*----------------------------------��������------------------------------------*/

 t2:printf("�ȴ�������\n");
    Length = sizeof(SOCKADDR);
    /*���ܿͻ��˵ķ������󣬵ȴ��ͻ��˷���connect����*/
    socket_receive = accept(socket_server, (SOCKADDR*)&Client_add, &Length);

    /*�����쳣���*/
    if (socket_receive == SOCKET_ERROR)
    {
        printf("����ʧ��! �Ƿ����÷���������Y��1/N��0��\n");
        scanf("%d", &Check1);
        if (Check1 == 1)
        {
            closesocket(socket_receive);
            closesocket(socket_server);
            WSACleanup();                           /*�ͷ��׽��֣��رն�̬��*/
            printf("���������óɹ�");
            goto t1;                                /*���÷�����*/
        }
        else
        {
            printf("�����˳��������������\n");
            _getch();
            closesocket(socket_receive);
            closesocket(socket_server);
            WSACleanup();                           /*�ͷ��׽��֣��رն�̬��*/
            return 0;                               /*�������*/
        }
    }
    else {
        printf("���ӳɹ�!\n(����_����ո�,��#END�����Ự)\n");
        fp = fopen("ServerData.txt", "a");          /*����־�ļ�*/
        time_t nowTime;
        time(&nowTime);
        sysTime = localtime(&nowTime);              /*��ȡϵͳʱ��*/

        fprintf(fp, "\n\n%d-%d-%d\t%d:%d:%d\n�˿�:%u\n", 1900 + sysTime->tm_year, sysTime->tm_mon + 1, sysTime->tm_mday,
            sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec, Point);
    }
    /*--------------------------------��������--------------------------------------*/
    while (1)
    {
        /*��������*/
        ReceiveLen = recv(socket_receive, Receivebuf, 100, 0);
        if (ReceiveLen < 0)
        {
            printf("����ʧ�ܣ��Ƿ����÷���������Y��1/N��0/WAIT��2��\n");
            scanf("%d", &Check1);
            if (Check1 == 1)
            {
                closesocket(socket_receive);
                closesocket(socket_server);
                WSACleanup();
                printf("���������óɹ�!\n");
                goto t1;                                        /*���÷�����*/
            }
            else if (Check1 == 2)
                goto t2;                                        /*���صȴ�*/
            else
            {
                printf("�����˳��������������\n");
                _getch();
                break;                                          /*��������˳�*/
            }
        }
        else
        {
            /*����Ƿ�Ϊ�����Ự��Ϣ*/
            if (strcmp(Receivebuf, "�Ự�ѽ���") == 0)
            {
                printf("�Ự�ѽ���,�Ƿ����÷���������Y��1/N��0/WAIT��2��\n");
                scanf("%d", &Check1);
                if (Check1 == 1)
                {
                    closesocket(socket_receive);
                    closesocket(socket_server);
                    WSACleanup();
                    printf("���������óɹ�");
                    goto t1;                                    /*���÷�����*/
                }
                else if (Check1 == 2)
                    goto t2;                                    /*���صȴ�*/
                else
                {
                    printf("�����˳��������������\n");
                    _getch();
                    break;                                      /*��������˳�*/
                }
            }
            /*��������������Ϣ��д����־*/
            else
            {
                printf("Client say: %s\n", Receivebuf);
                fprintf(fp, "Client say:%s\n", Receivebuf);
            }
            /*��������*/
            printf("please enter message:");
            scanf("%s", Sendbuf);                                   /*��ȡ�û��������Ϣ*/

            strcat(Sendbuf, "\0");                                  /*����ַ���������*/

            if (strcmp(Sendbuf, END) == 0)
            {
                printf("�Ự�ѽ���,�����������\n");
                fprintf(fp, "�Ự�ѽ���");                           /*д����־*/
                strcpy(Sendbuf, "�Ự�ѽ���\0");                     /*���ô�������Ϣ*/
                SendLen = send(socket_receive, Sendbuf, 100, 0);     /*���ͽ�����Ϣ*/
                _getch();
                break;                                               /*��������˳�*/
            }

            fprintf(fp, "Server say: %s\n", Sendbuf);                /*д����־*/

            SendLen = send(socket_receive, Sendbuf, 100, 0);         /*��������*/
            if (SendLen < 0)
            {
                printf("����ʧ�ܣ��Ƿ����÷���������Y��1/N��0/WAIT��2��\n");
                scanf("%d", &Check1);
                if (Check1 == 1)
                {
                    closesocket(socket_receive);
                    closesocket(socket_server);
                    WSACleanup();                                    /*�ͷ��׽��֣��رն�̬��*/
                    printf("���������óɹ�");
                    goto t1;                                         /*���÷�����*/
                }
                else if (Check1 == 2)
                    goto t2;                                         /*���صȴ�*/
                else
                {
                    printf("�����˳��������������\n");
                    _getch();
                    break;                                           /*��������˳�*/
                }
            }
        }
    }

    /*--------------------------------�����׶�----------------------------------*/

    /*�ͷ��׽��֣��رն�̬��*/
    closesocket(socket_receive);
    closesocket(socket_server);
    WSACleanup();
    return 0;                /*��������*/
}