#ignores=--ignore=txcouchbase
#pytest --annotate-output=annotations_generated.txt $ignores
#cp annotations_generated.txt annotations.txt
cat annotations_generated.txt | perl -pe 's|\(generated(.*?)\)||' > annotations.txt
subl annotations.txt
#sed -i.annotations.bak -E 's/\(generated(.*)?\)//' annotations.txt 
#rm annotations.bak
pyannotate -w --type-info annotations.txt couchbase
#find couchbase -name '*.py' | xargs -I% pyannotate --type-info annotations.txt %
