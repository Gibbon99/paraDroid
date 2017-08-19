.PHONY: clean All

All:
	@echo "----------Building project:[ paraDroid - Debug ]----------"
	@"$(MAKE)" -f  "paraDroid.mk"
clean:
	@echo "----------Cleaning project:[ paraDroid - Debug ]----------"
	@"$(MAKE)" -f  "paraDroid.mk" clean
