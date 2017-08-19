.PHONY: clean All

All:
	@echo "----------Building project:[ paraDroid - Release ]----------"
	@"$(MAKE)" -f  "paraDroid.mk"
clean:
	@echo "----------Cleaning project:[ paraDroid - Release ]----------"
	@"$(MAKE)" -f  "paraDroid.mk" clean
