set disassembly-flavor intel

define si
	stepi
	echo Registers:\n
	info register $eax $ebx $ecx $edx $esp $ebp $eip
	echo \n---\n\n
	echo Stack:\n
	x/32wx $esp
	echo \n---\n\n
	echo Following instructions:\n
	x/15i $eip
end

define ni
	nexti
	echo Registers:\n
	info register $eax $ebx $ecx $edx $esp $ebp $eip
	echo \n---\n\n
	echo Stack:\n
	x/32wx $esp
	echo \n---\n\n
	echo Following instructions:\n
	x/15i $eip
end

define inc
	x/42i $eip
end

define data
	print $arg0
	echo ---\n
	x/5s $arg0
	echo ---\n
	x/32wx $arg0
	echo ---\n
	x/10i $arg0
	echo ---\n
	x/16d $arg0
end

set breakpoint pending on
b main