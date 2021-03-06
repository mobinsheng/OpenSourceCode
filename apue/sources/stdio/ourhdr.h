apue/stdio/buf.c                                                                                       664     340      24         1435  5211774040   7103                                                                                                                                                                                                                                                                                                                                                                      #include	"ourhdr.h"
void	pr_stdio(const char *, FILE *);
int
main(void)
{
	FILE	*fp;

	fputs("enter any character\n", stdout);
	if (getchar() == EOF)
		err_sys("getchar error");
	fputs("one line to standard error\n", stderr);

	pr_stdio("stdin",  stdin);
	pr_stdio("stdout", stdout);
	pr_stdio("stderr", stderr);

	if ( (fp = fopen("/etc/motd", "r")) == NULL)
		err_sys("fopen error");
	if (getc(fp) == EOF)
		err_sys("getc error");
	pr_stdio("/etc/motd", fp);
	exit(0);
}
void
pr_stdio(const char *name, FILE *fp)
{
	printf("stream = %s, ", name);
				/* following is nonportable */
	if      (fp->_flag & _IONBF)	printf("unbuffered");
	else if (fp->_flag & _IOLBF)	printf("line buffered");
	else /* if neither of above */	printf("fully buffered");
	printf(", buffer size = %d\n", fp->_bufsiz);
}
status = -1;	/* probably out of processes */

	} else if (pid == 0) {			/* child */
			/* restore previous signal actions & reset signal mask */
		sigaction(SIGINT, &saveintr, NULL);
		sigaction(SIGQUIT, &savequit, NULL);
		sigapue/stdio/tempfiles.c                                                                                 664     340      24         1027  5211774041  10315                                                                                                                                                                                                                                                                                                                                                                      #include	"ourhdr.h"

int
main(void)
{
	char	name[L_tmpnam], line[MAXLINE];
	FILE	*fp;

	printf("%s\n", tmpnam(NULL));		/* first temp name */

	tmpnam(name);						/* second temp name */
	printf("%s\n", name);

	if ( (fp = tmpfile()) == NULL)		/* create temp file */
		err_sys("tmpfile error");
	fputs("one line of output\n", fp);	/* write to temp file */
	rewind(fp);							/* then read it back */
	if (fgets(line, sizeof(line), fp) == NULL)
		err_sys("fgets error");
	fputs(line, stdout);				/* print the line we wrote */

	exit(0);
}
%s, ", name);
				/* following is nonportable */
	if      (fp->_flag & _IONBF)	printf("unbuffered");
	else if (fp->_flag & _IOLBF)	printf("line buffered");
	else /* if neither of above */	printf("fully buffered");
	printf(", buffer size = %d\n", fp->_bufsiz);
}
status = -1;	/* probably out of processes */

	} else if (pid == 0) {			/* child */
			/* restore previous signal actions & reset signal mask */
		sigaction(SIGINT, &saveintr, NULL);
		sigaction(SIGQUIT, &savequit, NULL);
		sigapue/stdio/tempnam.c                                                                                   664     340      24          361  5211773634   7755                                                                                                                                                                                                                                                                                                                                                                      #include	"ourhdr.h"

int
main(int argc, char *argv[])
{
	if (argc != 3)
		err_quit("usage: a.out <directory> <prefix>");

	printf("%s\n", tempnam( argv[1][0] != ' ' ? argv[1] : NULL,
							argv[2][0] != ' ' ? argv[2] : NULL) );
	exit(0);
}
	/* create temp file */
		err_sys("tmpfile error");
	fputs("one line of output\n", fp);	/* write to temp file */
	rewind(fp);							/* then read it back */
	if (fgets(line, sizeof(line), fp) == NULL)
		err_sys("fgets error");
	fputs(line, stdout);				/* print the line weapue/stream