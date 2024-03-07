/*
 * atari_scsi.h -- Header file for the Atari native SCSI driver
 *
 * Copyright 1994 Roman Hodek <Roman.Hodek@informatik.uni-erlangen.de>
 *
 * (Loosely based on the work of Robert De Vries' team)
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file README.legal in the main directory of this archive
 * for more details.
 *
 */


#ifndef ATARI_SCSI_H
#define ATARI_SCSI_H

/* (I_HAVE_OVERRUNS stuff removed) */

#ifndef ASM
int atari_scsi_abort (Scsi_Cmnd *);
int atari_scsi_detect (Scsi_Host_Template *);
const char *atari_scsi_info (struct Scsi_Host *);
int atari_scsi_queue_command (Scsi_Cmnd *, void (*done) (Scsi_Cmnd *));
int atari_scsi_reset (Scsi_Cmnd *, unsigned int);
int atari_scsi_proc_info (char *, char **, off_t, int, int, int);
#ifdef MODULE
int atari_scsi_release (struct Scsi_Host *);
#else
#define atari_scsi_release NULL
#endif

/* The values for CMD_PER_LUN and CAN_QUEUE are somehow arbitrary. Higher
 * values should work, too; try it! (but cmd_per_lun costs memory!) */

/* But there seems to be a bug somewhere that requires CAN_QUEUE to be
 * 2*CMD_OER_LUN. At least on a TT, no spurious timeouts seen since
 * changed CMD_PER_LUN... */

/* Note: The Falcon currently uses 8/1 setting due to unsolved problems with
 * cmd_per_lun != 1 */

#define ATARI_TT_CAN_QUEUE		16
#define ATARI_TT_CMD_PER_LUN		8
#define ATARI_TT_SG_TABLESIZE		SG_ALL

#define ATARI_FALCON_CAN_QUEUE		8
#define ATARI_FALCON_CMD_PER_LUN	1
#define ATARI_FALCON_SG_TABLESIZE	SG_NONE

#define	DEFAULT_USE_TAGGED_QUEUING	0


#if defined (HOSTS_C) || defined (MODULE)

#define ATARI_SCSI { NULL, NULL, NULL,				\
  atari_scsi_proc_info,						\
  "Atari native SCSI",						\
  atari_scsi_detect,						\
  atari_scsi_release,						\
  atari_scsi_info,						\
  /* command */ NULL,						\
  atari_scsi_queue_command,					\
  atari_scsi_abort,						\
  atari_scsi_reset,						\
  /* slave_attach */	NULL,					\
  /* bios_param */	NULL,					\
  /* can queue */	0, /* initialized at run-time */	\
  /* host_id */		0, /* initialized at run-time */	\
  /* scatter gather */	0, /* initialized at run-time */	\
  /* cmd per lun */	0, /* initialized at run-time */	\
  /* present */		0,					\
  /* unchecked ISA DMA */ 0,					\
  /* use_clustering */	DISABLE_CLUSTERING }

#endif

#ifndef HOSTS_C

#define	NCR5380_implementation_fields	/* none */

#define NCR5380_read(reg)		  atari_scsi_reg_read( reg )
#define NCR5380_write(reg, value) atari_scsi_reg_write( reg, value )

#define NCR5380_intr atari_scsi_intr
#define NCR5380_queue_command atari_scsi_queue_command
#define NCR5380_abort atari_scsi_abort
#define NCR5380_proc_info atari_scsi_proc_info
#define NCR5380_dma_read_setup(inst,d,c) atari_scsi_dma_setup (inst, d, c, 0)
#define NCR5380_dma_write_setup(inst,d,c) atari_scsi_dma_setup (inst, d, c, 1)
#define NCR5380_dma_residual(inst) atari_scsi_dma_residual( inst )
#define	NCR5380_dma_xfer_len(i,cmd,phase) \
	atari_dma_xfer_len(cmd->SCp.this_residual,cmd,((phase) & SR_IO) ? 0 : 1)

#endif /* else def HOSTS_C */
#endif /* ndef ASM */
#endif /* ATARI_SCSI_H */


