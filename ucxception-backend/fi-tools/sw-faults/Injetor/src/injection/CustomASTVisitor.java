package injection;

import org.eclipse.cdt.core.dom.ast.*;

import java.util.ArrayList;
import java.util.Collections;

import static injection.FaultInjector.*;

// Visitor to navigate in tree and apply modifications
@SuppressWarnings("Duplicates")
public class CustomASTVisitor extends ASTVisitor {
    private IASTTranslationUnit translationUnit;
    private String operator;

    private int mifsFault = 0;
    private int miebFault = 0;
    private int miaFault  = 0;
    private int mlacFault = 0;
    private int mlocFault = 0;
    private int mfcFault  = 0;
    private int mlpaFault = 0;
    private int mvaeFault = 0;
    private int mvavFault = 0;
    private int mvivFault = 0;

    private int wvavFault = 0;
    private int waepFault = 0;
    private int wpfvFault = 0;

    private int walrFault = 0;
    private int wlecFault = 0;

    private int efcFault = 0;
    private int eifsFault = 0;

    private ArrayList<IASTNode> ids = new ArrayList<>();
    private ArrayList<IASTNode> vars = new ArrayList<>();
    private ArrayList<IASTNode> functions = new ArrayList<>();

    private String functionName = "";

    CustomASTVisitor(IASTTranslationUnit translationUnit, String operator) {
        this.translationUnit = translationUnit;
        this.operator = operator;
    }

    public int visit(IASTName name) {
        if ((name.getParent() instanceof IASTFunctionDeclarator)) {
            functionName = name.getRawSignature();
            functions.add(name.getParent());
        }
        return PROCESS_CONTINUE;
    }

    public int visit(IASTExpression expression) {
        if(expression instanceof IASTIdExpression){
            ids.add(expression.getChildren()[0]);
        }
        return PROCESS_CONTINUE;
    }

    public int visit(IASTDeclaration declaration) {
        if ((declaration instanceof IASTSimpleDeclaration)) {
            IASTSimpleDeclaration ast = (IASTSimpleDeclaration) declaration;
            IASTNode typedef = ast.getChildren().length == 1 ? ast.getChildren()[0] : ast.getChildren()[1];
            IASTNode[] children = typedef.getChildren();
            if ((children != null) && (children.length > 0))
            {
                ids.add(children[0]);
                vars.add(children[0].getParent().getParent());
            }
        }
        if ((declaration instanceof IASTFunctionDefinition)) {
            IASTFunctionDefinition ast = (IASTFunctionDefinition) declaration;

            ArrayList<Integer> toRemove = new ArrayList<>();
            for (int i = 0; i < vars.size(); i++) {
                if (!(vars.get(i).getParent() instanceof IASTTranslationUnit))
                {
                    toRemove.add(i);
                }
            }
            toRemove.sort(Collections.reverseOrder());
            for (Integer aToRemove : toRemove) {
                vars.remove(vars.get(aToRemove));
            }
            toRemove.clear();

            IASTFunctionDeclarator typedef = ast.getDeclarator();
            if (typedef.getChildren().length > 1)
            {
                for (int i = 1; i < typedef.getChildren().length; i++) {
                    ids.add(typedef.getChildren()[i].getChildren()[1].getChildren()[0]);
                    vars.add(typedef.getChildren()[i]);
                }
            }
        }
        return PROCESS_CONTINUE;
    }

    public int visit(IASTStatement statement) {

        // If is not compound statement and it was set a target line number, only inject that line
        if(FaultInjector.getTargetLine() == -1 || statement.getFileLocation().getStartingLineNumber() == FaultInjector.getTargetLine()) {
//		MISSING OPERATORS

//					MIFS
        if ((operator.isEmpty() || operator.compareTo(FaultInjector.Operators.MIFS.getSig()) == 0) && Constraints.c02(statement) && Constraints.c08(statement)
                // 0 is binaryExpression (condition) and 1 is the
                // compoundStatement
                && statement.getChildren().length > 1
                && Constraints.c09(statement.getChildren()[1])) {

            mifsFault = mifsVisit(translationUnit, statement, mifsFault);
        }

//					MIEB
        if ((operator.isEmpty() || operator.compareTo(FaultInjector.Operators.MIEB.getSig()) == 0) && Constraints.c08n(statement)) {
            miebFault = miebVisit(translationUnit, statement, miebFault);
        }

//					MIA
        if ((operator.isEmpty() || operator.compareTo(FaultInjector.Operators.MIA.getSig()) == 0) && Constraints.c08(statement)) {
            miaFault = miaVisit(translationUnit, statement, miaFault);
        }

//					MLAC
        if ((operator.isEmpty() || operator.compareTo(FaultInjector.Operators.MLAC.getSig()) == 0) && Constraints.c12(statement)) {
            mlacFault = mlacVisit(translationUnit, statement, mlacFault);
        }

//					MLOC
        if ((operator.isEmpty() || operator.compareTo(FaultInjector.Operators.MLOC.getSig()) == 0) && Constraints.c12(statement)) {
            mlocFault = mlocVisit(translationUnit, statement, mlocFault);
        }

//					MFC
        if ((operator.isEmpty() || operator.compareTo(FaultInjector.Operators.MFC.getSig()) == 0) && Constraints.c01(statement) && Constraints.c02(statement)) {
            mfcFault = mfcVisit(translationUnit, statement, mfcFault);
        }

//					MLPA
//					if (Constraints.c02(statement.getChildren()) && Constraints.c10(statement.getChildren())) {
        if (operator.isEmpty() || operator.compareTo(FaultInjector.Operators.MLPA.getSig()) == 0)
            mlpaFault = mlpaVisit(translationUnit, statement, mlpaFault);
//					}


//					MVAE
        if ((operator.isEmpty() || operator.compareTo(FaultInjector.Operators.MVAE.getSig()) == 0) && Constraints.c02(statement) && Constraints.c03(statement) && Constraints.c06(statement) && Constraints.c07(translationUnit, statement, functionName)) {
            mvaeFault = mvaeVisit(translationUnit, statement, mvaeFault);
        }

//					MVAV
        if ((operator.isEmpty() || operator.compareTo(FaultInjector.Operators.MVAV.getSig()) == 0) && Constraints.c02(statement) && Constraints.c03(statement) && Constraints.c06(statement) && Constraints.c07(translationUnit, statement, functionName)) {
            mvavFault = mvavVisit(translationUnit, statement, mvavFault);
        }

//					MVIV
        if ((operator.isEmpty() || operator.compareTo(FaultInjector.Operators.MVIV.getSig()) == 0) && Constraints.c02(statement) && Constraints.c03(statement) && Constraints.c04(translationUnit, statement, functionName) && Constraints.c05(statement) && Constraints.c06(statement)) {
            mvivFault = mvivVisit(translationUnit, statement, mvivFault);
        }

//		WRONG OPERATORS

//					WVAV
        if ((operator.isEmpty() || operator.compareTo(FaultInjector.Operators.WVAV.getSig()) == 0) && Constraints.c03(statement) && Constraints.c07(translationUnit, statement, functionName) && Constraints.c06(statement)) {
            wvavFault = wvavVisit(translationUnit, statement, wvavFault);
        }


//					WPFV
        if ((operator.isEmpty() || operator.compareTo(FaultInjector.Operators.WPFV.getSig()) == 0) && Constraints.c03(statement) && Constraints.c11(vars)) {
            wpfvFault = wpfvVisit(translationUnit, statement, wpfvFault, vars);
        }

//					WAEP
        if (operator.isEmpty() || operator.compareTo(FaultInjector.Operators.WAEP.getSig()) == 0) {
            waepFault = waepVisit(translationUnit, statement, waepFault);
        }

//                  WLEC
        if (operator.isEmpty() || operator.compareTo(FaultInjector.Operators.WLEC.getSig()) == 0) {
            wlecFault = wlecVisit(translationUnit, statement, wlecFault);
        }

//                  WALR
        if (operator.isEmpty() || operator.compareTo(FaultInjector.Operators.WALR.getSig())==0  && Constraints.c02(statement))
            walrFault = walrVisit(translationUnit, statement, walrFault);


//		EXTRA OPERATORS

        if(isExtraFocus()) {
//                  EFC
            if (operator.isEmpty() || operator.compareTo(FaultInjector.Operators.EFC.getSig()) == 0) {
                efcFault = efcVisit(translationUnit, statement, efcFault, functions, vars, functionName);
            }

/*                  EIFS - Still work in progress
            if ((operator.isEmpty() || operator.compareTo(FaultInjector.Operators.EIFS.getSig()) == 0) && Constraints.c11(vars)) {
                eifsFault = eifsVisit(translationUnit, statement, eifsFault, ids, vars, functionName);
            }
 */
        }
        }

        return PROCESS_CONTINUE;
    }
}
