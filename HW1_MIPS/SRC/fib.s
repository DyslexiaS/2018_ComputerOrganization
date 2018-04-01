# t0 = a
# t1 = b
# t2 = t
# t3 = i
# t4 = 1 or 0
.text
main:

	jal fib  # to fib function
	li   $v0, 10               # program stop
	syscall
fib:
	addi $t0, $t0, 1	# a=1
	addi $t3, $t3, 2	# i=2
for:
	add  $t2, $t0, $t1	# t=a+b
	add  $t1, $t0, $zero	# b=a
	add  $t0, $t2, $zero	# a=t
	
	addi $t3, $t3, 1	# i=i+1
	slti $t4, $t3, 26	# t4 = i<26 ?1 : 0
	bne  $t4, $0, for	# t4=1 goto for
    	jr $ra  		# return to the next line of jal

