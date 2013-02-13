all:
	make -C ../../elib/ 
	make -C ../../elib/ install
	make -C ../../test/ 

rebuild: clean all 

install:
	make -C ../../test/ install

uninstall:
	make -C ../../elib/ uninstall
	make -C ../../test/ uninstall 

clean:
	make -C ../../elib/ clean 
	make -C ../../test/ clean
