apue/signals/abort.c                                                                                   664     340      24         2115  5211774030   7747                                                                                                                                                                                                                                                                                                                                                                      #include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
abort(void)			/* POSIX-style abort() function */
{
	sigset_t			mask;
	struct sigaction	action;

		/* caller can't ignore SIGABRT, if so reset to default */
	sigaction(SIGABRT, NULL, &action);
	if (action.sa_handler == SIG_IGN) {
		action.sa_handler = SIG_DFL;
		sigaction(SIGABRT, &action, NULL);
	}

	if (action.sa_handler == SIG_DFL)
		fflush(NULL);			/* flush all open stdio streams */

		/* caller can't block SIGABRT; make sure it's unblocked */
	sigfillset(&mask);
	sigdelset(&mask, SIGABRT);	/* mask has only SIGABRT turned off */
	sigprocmask(SIG_SETMASK, &mask, NULL);

	kill(getpid(), SIGABRT);	/* send the signal */

		/* if we're here, process caught SIGABRT and returned */
	fflush(NULL);				/* flush all open stdio streams */

	action.sa_handler = SIG_DFL;
	sigaction(SIGABRT, &action, NULL);	/* reset disposition to default */
	sigprocmask(SIG_SETMASK, &mask, NULL);	/* just in case ... */

	kill(getpid(), SIGABRT);				/* and one more time */

	exit(1);	/* this should never be executed ... */
}
recognized option: -%c", optopt);
		}
	}
	if (optind >= argc)
		err_quit("usage: pty [ -d driver -einv ] program [ arg ... ]");

	if (interactive) {	/* fetch current termios and window size */
		if (tcgetattr(STDIN_FILENO, &orig_termios) < 0)
			err_sys("tcgetattr error on stdin");
		if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char *) &size) < 0)
			err_sys("TIOCGWINSZ error");
		pid = pty_fork(&fdm, slave_name, &orig_termios, &size);

	}apue/signals/child.c                                                                                   664     340      24         1202  5211774030   7717                                                                                                                                                                                                                                                                                                                                                                      #include	<sys/types.h>
#include	<signal.h>
#include	<stdio.h>

static void	sig_cld();

int
main()
{
	pid_t	pid;

	if (signal(SIGCLD, sig_cld) == -1)
		perror("signal error");

	if ( (pid = fork()) < 0)
		perror("fork error");
	else if (pid == 0) {		/* child */
		sleep(2);
		_exit(0);
	}
	pause();	/* parent */
	exit(0);
}

static void
sig_cld()
{
	pid_t	pid;
	int		status;

	printf("SIGCLD received\n");
	if (signal(SIGCLD, sig_cld) == -1)	/* reestablish handler */
		perror("signal error");

	if ( (pid = wait(&status)) < 0)		/* fetch child status */
		perror("wait error");
	printf("pid = %d\n", pid);
	return;		/* interrupts pause() */
}
G_SETMASK, &mask, NULL);

	kill(getpid(), SIGABRT);	/* send the signal */

		/* if we're here, process caught SIGABRT and returned */
	fflush(NULL);				/* flush all open stdio streams */

	action.sa_handler = SIG_DFL;
	sigaction(SIGABRT, &action, NULL);	/* reset disposition to default */
	sigprocmask(SIG_SETMASK, &mask, NULL);	/* just in case ... */

	kill(getpid(), SIGABRT);				apue/signals/critical.c                                                                                664     340      24         1721  5211774030  10434                                                                                                                                                                                                                                                                                                                                                                      #include	<s