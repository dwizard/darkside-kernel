ext2:
	# Build the MBR and VBR
	cd stage1; make all
	cd stage2; make ext2

	# Get a clean MBR image
	cp img/mbr_master.img img/mbr.img

	# Copy the MBR and VBR to the disk images
	dd if=stage1/build/stage1.bin of=img/mbr.img bs=440 conv=notrunc
	dd if=stage2/build/stage2.bin of=../img/images/ext2.img bs=1024 count=1 conv=notrunc
	./updategeorge.sh
	dd if=img/mbr.img of=../img/images/os.img bs=512 count=2048 conv=notrunc
