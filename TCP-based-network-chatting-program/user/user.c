#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<winsock.h> 
#include<time.h>
/*����������ͷ�ļ�*/

/*������t1�������÷�������ַλ��*/

int main()
/*���� winsockͷ�ļ�*/
{
    char Sendbuf[100];              /*�������ݵĻ�����*/
    char Receivebuf[100];           /*�������ݵĻ�����*/
    int SendLen;                    /*�������ݵĳ���*/
    int ReceiveLen;                 /*�������ݵĳ���*/
    int Check1;                     /*����ʧ���������*/
    unsigned short int Point;       /*�˿ں�*/
    char Address[20];               /*�洢IP��ַ*/
    char END[10] = "#END\0";        /*�洢������־�ַ�*/

    SOCKET socket_send;             /*�����׽���*/
    SOCKADDR_IN Server_add;         /*��������ַ��Ϣ�ṹ*/

    WORD wVersionRequested;         /*��(word):unsigned short*/
    WSADATA wsaData;                /*��汾��Ϣ�ṹ*/
    int error;                      /*��ʾ����*/

    FILE* fp;                       /*�ļ�ָ��*/
    struct tm* sysTime;             /*ϵͳʱ��ָ��*/

    /*---------------------------��ʼ���׽��ֿ�--------------------------------*/

    /*����汾���͡��������ֽ���ϳ�һ���֣�ǰ���ǵ��ֽڣ������Ǹ��ֽ�*/
    wVersionRequested = MAKEWORD(2, 2);
    /*�����׽��ֿ⣬��ʼ�� Ws232.dll��̬���ӿ�*/
    error = WSAStartup(wVersionRequested, &wsaData);
    if (error != 0)
    {
        printf("�����׽���ʧ�ܣ�\n");
        printf("�����˳��������������\n");
        _getch();
        return 0;                   /*�����������*/
    }

    /*�ж�������صİ汾���Ƿ����Ҫ��*/
    if(LOBYTE(wsaData.wVersion) != 2 ||
        HIBYTE(wsaData.wVersion) != 2)
    {
        WSACleanup();               /*�����ϣ��ر��׽��ֿ�*/
        printf("�������󣬳����˳��������������\n");
        _getch();           
        return 0;                   /*��������˳�*/
    }
    printf("��������ʮ����IP��ַ:");
    scanf("%s", Address);
    strcat(Address, "\0");
    printf("������˿ں�:");
    scanf("%hu", &Point);
    printf("����������%s,�˿ںţ�%d\n",Address,Point);

    /*-------------------------------���÷�������ַ-----------------------------------*/

 t1:Server_add.sin_family = AF_INET;  /*��ַ���壬������AF _INET,ע��ֻ�������������ֽ�˳��*/
    /*�������ĵ�ַ����һ�����ʮ���Ʊ�ʾΪIP��ַ��inet_ntoa�ǽ���ַת�����ַ���*/
    Server_add.sin_addr.S_un.S_addr = inet_addr(Address);
    Server_add.sin_port = htons(Point);/*�˿ں�*/

    /*--------------------------------�������ӷ�����-----------------------------------*/

    /*�ͻ��˴����׽��֣����ǲ���Ҫ�󶨣�ֻ��Ҫ�ͷ��������������ӾͿ�����*/
    /*socket_ sendr��ʾ�����׽��֣�Server_ add�Ƿ������ĵ�ַ�ṹ*/
    socket_send = socket(AF_INET, SOCK_STREAM, 0);

    /*-----------------------------�����������ӵ��׽���---------------------------------*/

    /*AF_INET��ʾָ����ַ�壬SOCK_STREAM��ʾ��ʽ�׽���TCP���ض��ĵ�ַ������ص�Э��*/

    /*�����쳣���*/
    if(connect(socket_send,(SOCKADDR*)&Server_add,sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        printf("����ʧ��! �Ƿ���������Y��1/N��0��\n");
        scanf("%d", &Check1);          
        if (Check1 == 1)
            goto t1;                   /*���ÿͻ���*/
        else
        {
            printf("�����˳��������������\n");
            _getch();                  /*��������˳�*/
            closesocket(socket_send);
            WSACleanup();              
            return 0;                  /*������ݣ���������*/
        }
    }
    /*�����������*/
    else {
        printf("���ӳɹ�!\n(����_����ո�,��#END�����Ự)\n");
        fp = fopen("UserData.txt", "a");                                /*����־�ļ�*/
        time_t nowTime;                                                 
        time(&nowTime);
        sysTime = localtime(&nowTime);                                  /*��ȡϵͳʱ��*/

        fprintf(fp, "\n\n%d-%d-%d\t%d:%d:%d\nIP:%s\n�˿�:%u\n",1900+sysTime->tm_year,sysTime->tm_mon+1,sysTime->tm_mday,
            sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec,Address,Point);
        /*������λỰ������Ϣ*/
    }
    /*---------------------------------��������-------------------------------------*/

    while (1) /*����ѭ��*/
    {
        /*�������ݹ���*/
        printf("please enter message:");
        scanf("%s", Sendbuf);                              /*��ȡ�û��������Ϣ*/

        strcat(Sendbuf, "\0");                             /*����ַ���������*/

        if (strcmp(Sendbuf, END) == 0)
        {
            printf("�Ự�ѽ���,�����������\n");            
            fprintf(fp, "�Ự�ѽ���\n");                    /*д����־*/
            strcpy(Sendbuf, "�Ự�ѽ���\0");                /*���ô�������Ϣ*/
            SendLen = send(socket_send, Sendbuf, 100, 0);   /*���ͽ�����Ϣ*/
            _getch();                                      
            break;                                          /*��������˳�*/
        }

        fprintf(fp,"Client say: %s\n", Sendbuf);            /*д����־*/
        
        SendLen = send(socket_send, Sendbuf, 100, 0);       /*��������*/
        /*����Ƿ����*/
        if (SendLen < 0)
        {
            printf("����ʧ�ܣ��Ƿ���������Y��1/N��0��\n");
            scanf("%d", &Check1);
            if (Check1 == 1)
                goto t1;                                     /*���ÿͻ���*/
            else
            {
                printf("�����˳��������������\n");
                _getch();
                break;                                       /*��������˳�*/
            }
        }

        /*�������ݹ���*/
        ReceiveLen = recv(socket_send, Receivebuf, 100, 0); /*��������*/
        if (ReceiveLen < 0)
        {
            printf("����ʧ��! �Ƿ���������Y��1/N��0��\n");
            scanf("%d", &Check1);
            if (Check1 == 1)
                goto t1;                                    /*���ÿͻ���*/
            else
            {
                printf("�����˳��������������\n");
                _getch();                                   /*��������˳�*/
                break;
            }
        }
        else
        {
            /*����Ƿ�Ϊ�����Ự��Ϣ*/
            if (strcmp(Receivebuf, "�Ự�ѽ���") == 0)
            {
                printf("�������ѹر�,��������رտͻ���\n");
                fprintf(fp, "�Ự�ѽ���\n");
                _getch();                                   
                break;                                      /*��������˳�*/
            }
            /*��������������Ϣ��д����־*/
            else
            {
                printf("Server say: %s\n", Receivebuf);
                fprintf(fp, "Server say:%s\n", Receivebuf);
            }
        }
    }

    /*-----------------------------�����׶�--------------------------------*/

    /*�ͷ��׽��֣��رն�̬��*/
    closesocket(socket_send);
    WSACleanup();
    fclose(fp);
    return 0;           /*��������*/
}