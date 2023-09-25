# DEADMAN(AGER)
*deadman* ("dead manager") is a Linux daemon that spawns and monitors a program *X*. If program *X* should terminate or be stopped
(SIGSTOP, SIGSTP) for ANY reason, a second program *Y* will be spawned as a failsafe. *deadman* also watches the filesystem to ensure
that the failsafe program *Y* is not removed. *deadman* is intended to be used as a dead man's switch.

# Usage
`./deadman PROGRAM_X PROGRAM_Y`

# Intended Features
- (--respawn, -r) flag to respawn the binary after termination is signaled
- Monitor filesystem to ensure failsafe program *Y* is not removed
- Forbid relative paths (to prevent path resolution attacks)

# NOTES TO SELF
Read chapter 26 ("Monitoring Child Processes") of the Linux Programming Interface
