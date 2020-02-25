# Partitions

* * *

This article is about the partitioning scheme that MS-DOS uses to keep track of it's partitions. This is the standard that all PC operation systems must use to be allowed to share hard disk resources with other operating systems.

* * *

DOS keeps track of partitions with a "Master Partition Table" that it keeps at offset 1BEh in the disk's boot sector. It contains four 16 byte entries, one for each partition. Each entry is divided into ten fields that contain all of the information necessary to describe a partition on the hard drive, including:

* Whether this partition is the one that got booted from.  

* The Head, Sector, and Cylinder where the partition starts.  

* The Head, Sector, and Cylinder where the partition ends.  

* The operating system that owns it.  

* The number of sectors that are before this partition.  

* The length of the partition in sectors.  

Now the astute reader may say: "Wait a minute! On my machine I run DOS/Windows and OS/2\. I have 5 DOS partitions. How can this be!"

Microsoft's solution to this problem was to invent the "Extended DOS" partition. If you go into FDISK, you will see that you have a "Primary" DOS partition, and an "Extended" DOS partition (Plus whatever else you have). You can also type in "UNFORMAT /PARTN /L" to get a list of all of your partitions. This clearly shows the chain format used.

* * *

<center>

<table border="" cellpadding="10" width="50%">

<tbody>

<tr align="CENTER">

<td></td>

<td colspan="3">Start</td>

<td></td>

<td colspan="3">End</td>

</tr>

<tr align="CENTER">

<td>BI</td>

<td>H</td>

<td>S</td>

<td>C</td>

<td>SI</td>

<td>H</td>

<td>S</td>

<td>C</td>

<td colspan="4">SB</td>

<td colspan="4">NS</td>

</tr>

<tr align="CENTER">

<td>0h</td>

<td>1h</td>

<td>2h</td>

<td>3h</td>

<td>4h</td>

<td>5h</td>

<td>6h</td>

<td>7h</td>

<td>8h</td>

<td>9h</td>

<td>Ah</td>

<td>Bh</td>

<td>Ch</td>

<td>Dh</td>

<td>Eh</td>

<td>Fh</td>

</tr>

</tbody>

</table>

</center>

This is repeated four times (one for each partition). Offsets are: 1BEh, 1CEh, 1DEh, and 1EEh. At offset 1FEh, AA55h signifies that the disk is bootable. The BIOS reads it in and executes it.

### Key:

*   For more, consult Ralf Brown's Interrupt list: INT 19  

* * *

*   Created by _[Chris Lattner](mailto:sabre@teleport.com)_