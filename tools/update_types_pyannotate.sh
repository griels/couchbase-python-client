#ignores=--ignore=txcouchbase
#pytest --annotate-output=annotations_generated.txt $ignores
#cp annotations_generated.txt annotations.txt
subl annotations.txt
sed -i -E 's/\(generated(.*)?\)//' annotations.txt 
find couchbase -name '*.py' | xargs -I% pyannotate --type-info annotations.txt %
