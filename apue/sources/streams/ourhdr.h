apue/streams/isastream.c                                                                               664     340      24          153  5211774041  10630                                                                                                                                                                                                                                                                                                                                                                      #include	<stropts.h>
#include	<unistd.h>

int
isastream(int fd)
{
	return(ioctl(fd, I_CANPUT, 0) != -1);
}
   Q  t_isastream.c ea} -o $@ t_isastream.o isastream.o ${LDLIBS}

clean:
	rm -f ${PROGS} ${TEMPFILES}
v[2] : NULL) );
	exit(0);
}
	/* create temp file */
		err_sys("tmpfile error");
	fputs("one line of output\n", fp);	/* write to temp file */
	rewind(fp);							/* then read it back */
	if (fgets(line, sizeof(line), fp) == NULL)
		err_sys("fgets error");
	fputs(line, stdout);				/* print the line weapue/streams/strlist.c                                                                                 644     340      24         1764  5211774042  10374                                                                                                                                                                                                                                                                                                                                                                      #include	<sys/conf.h>
#include	<sys/types.h>
#include	<fcntl.h>
#include	<stropts.h>
#include	"ourhdr.h"

int
main(int argc, char *argv[])
{
	int					fd, i, nmods;
	struct str_list		list;

	if (argc != 2)
		err_quit("usage: a.out <pathname>");

	if ( (fd = open(argv[1], O_RDONLY)) < 0)
		err_sys("can't open %s", argv[1]);
	if (isastream(fd) == 0)
		err_quit("%s is not a stream", argv[1]);

			/* fetch number of modules */
	if ( (nmods = ioctl(fd, I_LIST, (void *) 0)) < 0)
		err_sys("I_LIST error for nmods");
	printf("#modules = %d\n", nmods);

			/* allocate storage for all the module names */
	list.sl_modlist = calloc(nmods, sizeof(struct str_mlist));
	if (list.sl_modlist == NULL)
		err_sys("calloc error");
	list.sl_nmods = nmods;

			/* and fetch the module names */
	if (ioctl(fd, I_LIST, &list) < 0)
		err_sys("I_LIST error for list");

			/* print the module names */
	for (i = 1; i <= nmods; i++)
		printf("  %s: %s\n", (i == nmods) ? "driver" : "module",
								list.sl_modlist++);
	exit(0);
}
NULL);
		sigapue/streams/t_isastream.c                                                                             664     340      24          627  5211774043  11163                                                                                                                                                                                                                                                                                                                                                                      #include	<sys/types.h>
#include	<sys/fcntl.h>
#include	"ourhdr.h"

int
main(int argc, char *argv[])
{
	int		i, fd;

	for (i = 1; i < argc; i++) {
		printf("%s: ", argv[i]);
		if ( (fd = open(argv[i], O_RDONLY)) < 0) {
			err_ret("%s: can't open", argv[i]);
			continue;
		}

		if (isastream(fd) == 0)
			err_ret("%s: not a stream", argv[i]);
		else
			err_msg("%s: streams device", argv[i]);
	}
	exit(0);
}
mber of modules */
	if ( (nmods = ioctl(fd, I_LIST, (void *) 0)) < 0)
		err_sys("I_LIST error for nmods")apue/termios/                                                                                          775     340      24            0  5211773637   6446                                                                                                                                                                                                                                                                                                                                                                      apue/termio