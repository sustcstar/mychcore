/*
 * Copyright (c) 2020 Institute of Parallel And Distributed Systems (IPADS), Shanghai Jiao Tong University (SJTU)
 * OS-Lab-2020 (i.e., ChCore) is licensed under the Mulan PSL v1.
 * You can use this software according to the terms and conditions of the Mulan PSL v1.
 * You may obtain a copy of Mulan PSL v1 at:
 *   http://license.coscl.org.cn/MulanPSL
 *   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 *   PURPOSE.
 *   See the Mulan PSL v1 for more details.
 */

// Simple command-line kernel monitor useful for
// controlling the kernel and exploring the system interactively.

#include <common/printk.h>
#include <common/types.h>

static inline __attribute__ ((always_inline))
u64 read_fp()
{
	u64 fp;
	__asm __volatile("mov %0, x29":"=r"(fp));
	return fp;
}

__attribute__ ((optimize("O1")))
int stack_backtrace()
{
	printk("Stack backtrace:\n");

	// Your code here.
	u64* fp=(u64*)(*(u64*)read_fp());	// 输出的FP为调用stack_backtrace的函数的FP，故加一层间接访问
	while(*(fp) != 0){	// 递归到没有父函数时停止
        // 地址为FP+8处的值为当前函数LR，地址为FP处的值为父函数的FP，FP的值就是当前函数的FP
		printk("LR %lx FP %lx Args ",*(fp+1),fp);	
		u64* p=fp-2; // 地址为FP-16处开始的值为当前函数的参数列表
		for(int k=5;k>0;k--){
			printk("%d ",*p);
			p++;
		}
		printk("\n");
		fp = (u64*) *fp; // 沿着FP递归访问
	}

	return 0;
}
