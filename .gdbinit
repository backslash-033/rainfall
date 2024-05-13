set disassembly-flavor intel

define si
	echo Registers:\n
	info register $eax $ebx $ecx $edx $esp $ebp $eip
	echo \n---\n\n
	echo Stack:\n
	x/64x $esp
	echo \n---\n\n
	echo Following instructions:\n
	x/15i $eip
	nexti
end

define ni
	echo Registers:\n
	info register $eax $ebx $ecx $edx $esp $ebp $eip
	echo \n---\n\n
	echo Stack:\n
	x/64x $esp
	echo \n---\n\n
	echo Following instructions:\n
	x/15i $eip
	nexti
end

define inc
	x/42i $eip
end

define data
	print $arg0
	echo ---\n
	x/5s $arg0
	echo ---\n
	x/64x $arg0
	echo ---\n
	x/10i $arg0
	echo ---\n
	x/64d $arg0
end

set breakpoint pending on
b main