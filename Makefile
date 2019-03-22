compileWrite :
	h5c++ Write.cpp -o writexe
	@echo "Write File has been compiled, executable file generated"
	@echo "Executing"
	./writexe
	@echo "H5 FIle generated as output"
compileRead :	
	h5c++ Read.cpp -o readexe
	@echo "Read File has been compiled, executable file generated" 
	./readexe
	@echo "\nRead File successful"
