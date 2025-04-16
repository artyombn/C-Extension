clear_so:
	rm -rf build *.egg-info dist *.so

build_local: clear_so
	python3 setup.py build_ext --inplace

run_test:
	python3 test.py