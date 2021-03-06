/* $Id$
 * Library interface to global-mode performance counters.
 *
 * Copyright (C) 1999-2003  Mikael Pettersson
 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "libperfctr.h"
#include "marshal.h"

struct gperfctr {	/* XXX: kill this struct */
    int fd;
};

struct gperfctr *gperfctr_open(void)
{
    struct gperfctr *gperfctr;

    gperfctr = malloc(sizeof(*gperfctr));
    if( gperfctr ) {
	gperfctr->fd = open("/dev/perfctr", O_RDONLY);
	if( gperfctr->fd >= 0 ) {
	    if( perfctr_abi_check_fd(gperfctr->fd) >= 0 )
		return gperfctr;
	    close(gperfctr->fd);
	}
	free(gperfctr);
    }
    return NULL;
}

void gperfctr_close(struct gperfctr *gperfctr)
{
    close(gperfctr->fd);
    free(gperfctr);
}

int gperfctr_control(const struct gperfctr *gperfctr,
		     struct gperfctr_cpu_control *arg)
{
    return perfctr_ioctl_w(gperfctr->fd, GPERFCTR_CONTROL, arg,
			   &gperfctr_cpu_control_sdesc);
}

int gperfctr_read(const struct gperfctr *gperfctr, struct gperfctr_cpu_state *arg)
{
    return perfctr_ioctl_wr(gperfctr->fd, GPERFCTR_READ, arg,
			    &gperfctr_cpu_state_only_cpu_sdesc,
			    &gperfctr_cpu_state_sdesc);
}

int gperfctr_stop(const struct gperfctr *gperfctr)
{
    return ioctl(gperfctr->fd, GPERFCTR_STOP, 0);
}

int gperfctr_start(const struct gperfctr *gperfctr, unsigned int interval_usec)
{
    return ioctl(gperfctr->fd, GPERFCTR_START, interval_usec);
}

int gperfctr_info(const struct gperfctr *gperfctr, struct perfctr_info *info)
{
    return perfctr_info(gperfctr->fd, info);
}

struct perfctr_cpus_info *gperfctr_cpus_info(const struct gperfctr *gperfctr)
{
    return perfctr_cpus_info(gperfctr->fd);
}
