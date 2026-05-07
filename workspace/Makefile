.PHONY: clean init test tidiness-check 

DIRECTORY_BUILD = build/
BUILD_SYSTEM="Ninja"
BUILD_SYSTEM_COMMAND=ninja

clean:
	rm -rf ${DIRECTORY_BUILD}

init: clean
	cmake -S . -B ${DIRECTORY_BUILD} -G ${BUILD_SYSTEM}

test:
	cd ${DIRECTORY_BUILD} && ${BUILD_SYSTEM_COMMAND} run_tests

tidiness-check: test
	git push origin main --dry-run

upload:
	git push origin main
