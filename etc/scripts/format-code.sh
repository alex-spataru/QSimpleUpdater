# Style and format recursively
astyle --style=google --indent=spaces --align-pointer=type  --remove-brackets --convert-tabs --close-templates --max-code-length=80 --max-instatement-indent=50 --lineend=windows --suffix=none --recursive ../../*.h ../../*.cpp ../../*.c
