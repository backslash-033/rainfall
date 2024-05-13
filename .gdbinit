set disassembly-flavor intel

define si
	x/15i $eip
	stepi
end

define ni
	x/15i $eip
	nexti
end
