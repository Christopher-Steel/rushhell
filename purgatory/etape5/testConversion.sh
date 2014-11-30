./build.sh \
 && ./test_nsa_to_dfa \
 && dot -Tpdf NFA.dot -o DFA.pdf \
 && dot -Tpdf DFA.dot -o DFA.pdf \
 && google-chrome NFA.pdf DFA.pdf
