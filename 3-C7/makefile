all:
	@echo "Start compile library..."
	make -C src
	@echo "Start compile test program..."
	make -C text

run:
	@echo "Start run test program..."
	make run -C text 

clean:
	make clean -C src 
	make clean -C text 

distclean: clean 
	rm -f lib/*.a
	rm -f lib/*.so 

