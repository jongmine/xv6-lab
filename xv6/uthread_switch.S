	.text

	/*
     * save the old thread's registers,
     * restore the new thread's registers.
     */

	.globl thread_switch
thread_switch:
	/* YOUR CODE HERE */
  	# 현재 스레드의 스택 포인터(%esp)를 current_thread->sp에 저장
   	movl %esp, (%eax)

   	# next_thread의 스택 포인터를 %esp에 복원
   	movl (%edx), %esp   # %edx에는 next_thread 포인터가 들어 있다고 가정

   	# 이후 작업을 위해 current_thread를 next_thread로 업데이트
   	movl %edx, %eax     # 이제 %eax(current_thread)는 새로운 스레드를 가리킴
	ret    /* return to ra */
