###################################################
##             ASU CSE 571 ONLINE                ##
##        Unit 3 Reasoning under Uncertainty     ##
##             Project Submission File           ##
##                 burglary.py                   ##
###################################################

###################################################
##                !!!IMPORTANT!!!                ##
##        This file will be auto-graded          ##
##    Do NOT change this file other than at the  ##
##       Designated places with your code        ##
##                                               ##
##  READ the instructions provided in the code   ##
###################################################

# Starting with defining the network structure
from pgmpy.models import BayesianModel
from pgmpy.factors.discrete import TabularCPD
from pgmpy.inference import VariableElimination

def buildBN():

    #!!!!!!!!!!!!!!!  VERY IMPORTANT  !!!!!!!!!!!!!!!
    # MAKE SURE to use the terms "MaryCalls", "JohnCalls", "Alarm",
    # "Burglary" and "Earthquake" as the states/nodes of the Network.
    # And also use "burglary_model" as the name of your Bayesian model.
    ########-----YOUR CODE STARTS HERE-----########



    
    ########-----YOUR CODE ENDS HERE-----########
    
    # Doing exact inference using Variable Elimination
    burglary_infer = VariableElimination(burglary_model)

    ########-----YOUR MAY TEST YOUR CODE BELOW -----########
    ########-----ADDITIONAL CODE STARTS HERE-----########




    ########-----YOUR CODE ENDS HERE-----########
    
    return burglary_infer