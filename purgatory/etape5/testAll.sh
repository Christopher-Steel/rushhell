./build.sh \
 && ./test_opdot_fsa \
 && dot -Tpdf matcher.dot -o matcher.pdf \
 && dot -Tpdf NFA.dot -o NFA.pdf \
 && dot -Tpdf DFA.dot -o DFA.pdf \
 && dot -Tpdf appended.dot -o appended.pdf \
 && dot -Tpdf merged1.dot -o merged1.pdf \
 && dot -Tpdf merged2.dot -o merged2.pdf \
 && google-chrome matcher.pdf NFA.pdf DFA.pdf appended.pdf merged1.pdf merged2.pdf
