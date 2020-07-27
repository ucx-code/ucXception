package injection;

import org.eclipse.cdt.core.dom.ast.*;

import java.util.ArrayList;

public class Constraints{
/*	 Constraints
	 c01 - done
	 c02 - done
	 c03 - done
	 c04 - done
	 c05 - done
	 c06 - done
	 c07 - done
	 c08 - done
	 c09 - done
	 c10 - done
	 c11
	 c12 - done
*/

		/**
		 * Return if there is a function call
		 * @param node to verify
		 * @return
		 */
		static boolean c01(IASTNode node) {
			if (Utilities.isExpressionStatement(node) && node.getChildren().length == 1) {
				IASTNode[] children = node.getChildren();
				if (children[0] instanceof IASTFunctionCallExpression)
					return true;
				// Retirar isto porque -> A function call is a postfix expression followed by parentheses containing a
				// possibly empty, comma-separated list of expressions which constitute the arguments to the function.
				else if(children[0] instanceof IASTBinaryExpression && children[0].getChildren()[1] instanceof IASTFunctionCallExpression)
					return true;
			}
			return false;
		}

		/**
		 * Need to have more than 1 statement in block
		 * @param node to verify
		 * @return
		 */
		static boolean c02(IASTNode node)
		{
			IASTNode parent = node.getParent();
			if (parent != null && parent.getChildren() != null && Utilities.isForStatement(parent) && node.getChildren().length > 1)
				return true;
			if (parent != null && parent.getChildren() != null && parent.getChildren().length > 1 && !Utilities.isForStatement(parent))
				return true;
			return false;
		}

//		public static boolean c02(IASTNode[] node)
//		{
//			IASTNode parent = node[0].getParent();
//			if (parent != null && parent.getChildren() != null && parent.getChildren().length > node.length)
//				return true;
//			return false;
//		
//		}

		/**
		 * basta que nÃ£o seja variavel global?
		 * @param node
		 * @return
		 */
		static boolean c03(IASTNode node) {
			IASTNode parent = node.getParent();
			if (parent != null && !Utilities.isSimpleDeclaration(parent))
				return true;
			return false;
		}

		/**
		 * Must be the first assignment for that variable in the module
		 * @param node
		 * @return
		 */
		static boolean c04(IASTTranslationUnit translationUnit, IASTNode node, String functionName) {
			String varName = null;
//			int w = 0;
//			SimpleDeclSpecifier Declarator EqualsInitializer LiteralExpression
//			int w = global - 0;
//			SimpleDeclSpecifier Declarator EqualsInitializer BinaryExpression
			IASTNode[] children = node.getChildren();
//			DeclarationStatement
			if (Utilities.isDeclarationStatement(node) && children.length == 1)
			{
//				SimpleDeclaration
				if (children[0] != null && Utilities.isSimpleDeclaration(children[0]) && children[0].getChildren().length == 2)
				{
					children = children[0].getChildren();
//					0 SimpleDeclSpecifier
//					1 Declarator
					if (children[1] != null && Utilities.isDeclarator(children[1]) && children[1].getChildren().length == 2)
					{
//						0 Name
//						1 EqualsInitializer
						
//						System.out.println("Ã© uma declaration");
						return true;
					}
					else if (children[1] != null && Utilities.isDeclarator(children[1]) && children[1].getChildren().length == 1)
					{
						children = children[1].getChildren();
						varName = children[0].getRawSignature();
					}
				}
			}
//			ExpressionStatement
			if (Utilities.isExpressionStatement(node) && children.length == 1)
			{
//				System.out.println("testing");
				children = node.getChildren();
//				BinaryExpression
				if (children != null && children.length == 1 && Utilities.isBinaryExpression(children[0]))
				{
//					IdExpression LiteralExpression
//					IdExpression BinaryExpression
					children = children[0].getChildren();
					varName = children[0].getRawSignature();
				}
			}
			
//			System.out.println(varName);
			
//			int w;
//			w = 0;
			
//			TODO
//			garantir que na sua declaraÃ§Ã£o inicial, nÃ£o foi feita a sua inicializaÃ§Ã£o (de forma recursiva)
//			Procurar o name na arvore de codigo e verificar se Ã© a sua primeira inicializaÃ§Ã£o!!!
			boolean result = false;
			if (varName != null) {
				result = recursiveSearchAssignment(translationUnit, varName, functionName, node.getFileLocation().getNodeOffset(), true);
				return result;
			}
			return false;
		}
		
		
//		pesquisa dentro da funÃ§Ã£o? ou tambem globalmente?
		private static boolean recursiveSearchAssignment(IASTNode node, String varName, String functionName, int offset, boolean must) {
			
//			procura a declaraÃ§Ã£o da variavel
			IASTNode[] children = node.getChildren();
//			DeclarationStatement
			if (Utilities.isDeclarationStatement(node) && children.length == 1)
			{
//				SimpleDeclaration
				if (children[0] != null && Utilities.isSimpleDeclaration(children[0]) && children[0].getChildren().length == 2)
				{
					children = children[0].getChildren();
//					0 SimpleDeclSpecifier
//					1 Declarator
					if (children[1] != null && Utilities.isDeclarator(children[1]) && children[1].getChildren().length == 2)
					{
//						0 Name
//						1 EqualsInitializer
						children = children[1].getChildren();
						if (children[0]!= null && children[0].getRawSignature().compareTo(varName) == 0)
						{
//							System.out.println("encontra a declaraÃ§Ã£o da variavel com inicializaÃ§Ã£o");
							return true;
						}
					}
				}
			}
			
//			procura inicializaÃ§Ã£o, sem declaraÃ§Ã£o
//			Expression
			if (Utilities.isExpressionStatement(node) && children.length == 1)
			{
//				BinaryExpression
				if (children[0] != null && Utilities.isBinaryExpression(children[0]) && children[0].getChildren().length == 2)
				{
					children = children[0].getChildren();
//					0 IdExpression
//					1 LiteralExpression | BinaryExpression
					if (children[1] != null && (Utilities.isLiteralExpression(children[1]) || Utilities.isBinaryExpression(children[1]) || Utilities.isIdExpression(children[1])) && children[0].getChildren().length == 1)
//						if (children[1] != null && (Utilities.isLiteralExpression(children[1]) || Utilities.isBinaryExpression(children[1])) && children[0].getChildren().length == 1)
					{
//						0 Name
						children = children[0].getChildren();
						if (children[0].getRawSignature().compareTo(varName) == 0)
						{
//							System.out.println("encontra a inicializaÃ§Ã£o da variavel");
//							System.out.println(children[0].getFileLocation().getNodeOffset());
//							System.out.println(offset);
							
							if (!must && children[0].getFileLocation().getNodeOffset() < offset){
								return true;
							}
						}
					}
				}
			}
			
			for (IASTNode iastNode : children) {
				if (Utilities.isFunctionDeclarator(iastNode)){
					IASTNode[] functionDeclarator = iastNode.getChildren();
					if (functionDeclarator[0].getRawSignature().compareTo(functionName) != 0){
						continue;
					}
				}
				if (recursiveSearchAssignment(iastNode, varName, functionName, offset, must))
				{
					return true;
				}
			}
			return false;
		}

		/**
		 * Assignment must not be inside a loop
		 * @param node to verify
		 * @return true if not have any parent that is a loop
		 */
		static boolean c05(IASTNode node) {
			IASTNode parent = node.getParent();
			while(parent != null)
			{
				if (Utilities.isLoop(parent)) return false;
				parent = parent.getParent();
			}
			return true;
		}

		/**
		 * Assignment must not be a part of a for construct
		 * @param node to verify
		 * @return true if it isn't a part of a for construct
		 */
		static boolean c06(IASTNode node) {
			IASTNode parent = node.getParent();
			if (parent != null && !Utilities.isForStatement(parent))
				return true;
			return false;
		}

		static boolean c07(IASTTranslationUnit translationUnit, IASTNode node, String functionName) {
//			nao pode ser declaraÃ§Ã£o
//			verifica se jÃ¡ teve algum assignment antes!!!
			
//			int w;
//			w = 0;
			if (Utilities.isExpressionStatement(node))
			{
				IASTNode[] children = node.getChildren();
				if (children.length == 1 && Utilities.isBinaryExpression(children[0]))
				{
					IASTNode[] grandchildren =  children[0].getChildren();
					if (grandchildren.length == 2)
					{
						String varName = grandchildren[0].getRawSignature();
                        return recursiveSearchAssignment(translationUnit, varName, functionName, node.getFileLocation().getNodeOffset(), false);
					}
				}
			}
			return false;
		}

		/**
		 * Must not be associated to an else construct, have only a BinaryExpression and a CompoundStatement
		 * @param node
		 * @return
		 */
		static boolean c08(IASTNode node) {
			IASTNode[] children = node.getChildren();

//			TODO clean
			/*//		The parent must not be an IfStatement*/
			if (children.length <= 2 /*&& !Utilities.isIfStatement(node.getParent())*/)
				return true;
			return false;
		}

		/**
		 * Must be associated to an else construct, have 3 childrens or more
		 * @param node
		 * @return
		 */
		static boolean c08n(IASTNode node)
		{
			IASTNode[] children = node.getChildren();

//			TODO clean
			/*//		The parent must not be an IfStatement*/
			if (children.length > 2 /*&& !Utilities.isIfStatement(node.getParent())*/)
				return true;
			return false;
		}

		/**
		 * Must not include more than five statements and not include loops
		 * @param node
		 * @return if constrain is check
		 */
		static boolean c09(IASTNode node)
		{
			if (node.getChildren().length <= 5 && !Utilities.isLoop(node, 1))
				return true;
			return false;
		}

		/**
		 * Statements are in the same block, do not include more than five statements or loops
		 * missing statements are contiguous and belong to the same code block
		 * @param nodes
		 * @return
		 */
		public static boolean c10(IASTNode[] nodes) {
//			System.out.println("+" + Utilities.isCompoundStatement(node));
			if (Utilities.isCompoundStatement(nodes[0].getParent()) && nodes.length <= 5)
			{
				for (int i = 0; i < nodes.length; i++) {
					if (Utilities.isLoop(nodes[i], 1) && Utilities.isIfStatement(nodes[i]))
					{
						return false;
					}
				}
				return true;
			}
			return false;
		}

		/**
		 * There must be at least two variables in this module
		 * @param vars Array with visible variables
		 * @return
		 */
		static boolean c11(ArrayList<IASTNode> vars) {
			if (vars.size() >= 2)
				return true;
			return false;
		}

		/**
		 * Must have at least two branch conditions
		 * @param node
		 * @return
		 */
		static boolean c12(IASTNode node) {
			if (node instanceof IASTIfStatement) {
				IASTIfStatement newStatement = (IASTIfStatement) node;
				if (newStatement.getConditionExpression() != null && newStatement.getConditionExpression().getChildren() != null)
				{
					IASTNode[] conditionChildrens = newStatement.getConditionExpression().getChildren();
					if (conditionChildrens.length >= 2)
						return true;
				}
			}
			if (node instanceof IASTWhileStatement) {
				IASTWhileStatement newStatement = (IASTWhileStatement) node;
				if (newStatement.getCondition() != null && newStatement.getCondition().getChildren() != null)
				{
					IASTNode[] conditionChildrens = newStatement.getCondition().getChildren();
					if (conditionChildrens.length >= 2)
						return true;
				}
			}
			if (node instanceof IASTDoStatement) {
				IASTDoStatement newStatement = (IASTDoStatement) node;
				if (newStatement.getCondition() != null && newStatement.getCondition().getChildren() != null)
				{
					IASTNode[] conditionChildrens = newStatement.getCondition().getChildren();
					if (conditionChildrens.length >= 2)
						return true;
				}
			}
			return false;
		}
}