# C_project_2_expression_checker

This project checks for balanced expressions and returns whether the expression is balanced or not

EX:

( ( a a ) < > [ [ [ { [ x ] } ]]] <>)
Expression is balanced
( ( a a ) < > [ [ [ { [ x ] ]]] <>)
                           
                            ^ expecting }

( ( a a ) ) < > > [ [ [ { [ x ] } ]]] <>)
 
                ^ missing <

( ( a a ) < > [ [ [ { [ x ] } ]]]
 
                                 ^ missing )
