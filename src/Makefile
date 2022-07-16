TEST_FLAGS=-I -L -lcheck `pkg-config --cflags --libs check` -lm -lpthread
TEST_C=s21_calc_v1_0_main.c s21_calc_v1_0_test.c
EXECUTABLE=s21_calc_v1_0_test.out
CPP_FLAGS=-Wall -Wextra -Werror
GCOV_FLAG=--coverage
DB_FLAG=-g
CC=gcc -std=c11
LINTCFG_WAY=../materials/linters/CPPLINT.cfg
LINT_WAY=../materials/linters/cpplint.py
EXTENSION=--extensions=c
LINTCFG=CPPLINT.cfg
CHECK_FILES=s21*.c s21*.h
CPPCH=cppcheck
CPPCH_FLAGS=--enable=warning,performance,portability --std=c11 -v
PY=python3
COPY=cp -r
VALGRIND_TOOL_OPTION=--leak-check=full
VALGRIND_COMMAND=CK_FORK=no valgrind
LEAKS_COMMAND=leaks -atExit --
VALGRIND_TOOL=--tool=memcheck
COV_TEST_NAME=-t "tests"
COV_RESULT=test.info
COV_CAPTURE_FLAG=-c
COV_DIR=-d .
COV=lcov
HTML=genhtml
HTML_RESULT_DIR=-o report
MAKE_DIR=mkdir -p
CBUILDER=cmake
BUILDER=make
SOURCE=s21_calc
BUILD_DIR=../build
APP_PACKAGE=s21_calc.app/Contents/MacOS/s21_calc
QCP_LIB=../materials/QCustomPlot-library-master
CH_DIR=cd
BUILD_FILES_TO_DELETE=`ls | grep -v s21_calc.app`
MAKEINFO=makeinfo --html
DOC_DIR=../documentation
DOC_NAME=index.html
TEXI_INPUT=dvi/info.texi
TAR_NAME=$(BUILD_DIR)/Calculator-v1_0.tar
ARCHIVER=bash archiver.sh
APP=s21_calc.app
PLIST=$(SOURCE)/Info.plist
ICON_RESOURCES=s21_calc/Resources
ICON_RESOURCES_TARGET=$(BUILD_DIR)/$(APP)/Contents/
GIVE_RIGHT=chmod -R u+wrx


TO_DELETE=*.o *.a *.gcda *.gcno *.info report *.dSYM a.out t r $(BUILD_DIR) $(DOC_DIR)
DELETE=rm -rf

all: gcov_report check tests dvi dist run

install:
	$(MAKE_DIR) $(BUILD_DIR)
	$(CBUILDER) -S $(SOURCE) -B $(BUILD_DIR)
	$(COPY) $(QCP_LIB) $(BUILD_DIR)
	$(CH_DIR) $(BUILD_DIR) && $(BUILDER) && $(DELETE) $(BUILD_FILES_TO_DELETE)
	$(GIVE_RIGHT) $(BUILD_DIR)
	$(COPY) $(ICON_RESOURCES) $(ICON_RESOURCES_TARGET)
	$(COPY) $(PLIST) $(ICON_RESOURCES_TARGET)
	open $(BUILD_DIR)

uninstall:
	$(DELETE) $(BUILD_DIR)/$(APP)

dvi: get_html
	open $(DOC_DIR)/$(DOC_NAME)

get_html:
	$(MAKEINFO) -o $(DOC_DIR) $(TEXI_INPUT)

dist:	
	$(ARCHIVER) $(TAR_NAME)

check:
	$(CPPCH) $(CPPCH_FLAGS) $(CHECK_FILES)
	$(COPY) $(LINTCFG_WAY) $(LINTCFG)
	$(PY) $(LINT_WAY) $(EXTENSION) $(CHECK_FILES)

run:
	./$(BUILD_DIR)/$(APP_PACKAGE)

tests: clean
	$(CC) $(DB_FLAG) $(GCOV_FLAG) $(TEST_C) -o $(EXECUTABLE) $(TEST_FLAGS)
	./$(EXECUTABLE)

gcov_report: tests
	$(COV) $(COV_TEST_NAME) -o $(COV_RESULT) $(COV_CAPTURE_FLAG) $(COV_DIR)
	$(HTML) $(HTML_RESULT_DIR) $(COV_RESULT)

valgrind: tests
	$(VALGRIND_COMMAND) $(VALGRIND_TOOL) $(VALGRIND_TOOL_OPTION) ./$(EXECUTABLE)

leaks: tests
	$(LEAKS_COMMAND) ./$(EXECUTABLE)

clean:
	$(DELETE) $(TO_DELETE) $(EXECUTABLE) $(LINTCFG)
