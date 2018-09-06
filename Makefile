clang-format:
	find src/ -iname *.h -o -iname *.cpp | xargs clang-format -style=google -i
