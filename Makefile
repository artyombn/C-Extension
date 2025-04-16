clear_so:
	rm -rf build *.egg-info dist *.so

build_local: clear_so
	python3 setup_protobuf.py build_ext --inplace

run_test:
	python3 c_protobuf_test.py