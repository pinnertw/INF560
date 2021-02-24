all:hello_cpu hello_gpu

hello_cpu: hello_cpu.c
	gcc -o $@ $^

hello_gpu : hello_gpu.cu
	nvcc -o $@ $^

clean:
	rm -f hello_cpu
