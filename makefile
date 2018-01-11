cc = gcc
prom = main
#using find to save list adding time
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.c")
#replace the char cpp in src to o
obj = $(src:%.c=%.o)
gtk = `pkg-config --cflags --libs gtk+-3.0`

$(prom): $(obj)
	$(cc) -ansi -o $(prom) $(obj) $(gtk)

%.o: %.c $(deps)
	$(cc) -ansi -c $< -o $@ $(gtk)

clean:
	rm -rf $(obj) $(prom)cc = gcc
