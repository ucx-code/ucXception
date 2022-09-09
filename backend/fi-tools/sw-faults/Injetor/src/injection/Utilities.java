package injection;

import org.eclipse.cdt.core.dom.ast.IASTNode;
import org.eclipse.cdt.core.dom.ast.IASTTranslationUnit;
import org.eclipse.cdt.core.dom.ast.gnu.c.GCCLanguage;
import org.eclipse.cdt.core.dom.ast.gnu.cpp.GPPLanguage;
import org.eclipse.cdt.core.parser.IParserLogService;
import org.eclipse.cdt.core.parser.IScannerInfo;
import org.eclipse.cdt.core.parser.IncludeFileContentProvider;
import org.eclipse.cdt.internal.core.parser.scanner.InternalFileContent;
import org.eclipse.core.runtime.CoreException;

class Utilities {

	/**
	 * According to whether we want the C or the CPP parser, return the corresponding IASTTranslationUnit
	 * @param c_parser - True if we want the C parser, False if we want the CPP
	 * @param internalFileContent - Internal file content
	 * @param info - Scanner info
	 * @param emptyIncludes
	 * @param opts
	 * @param log
	 * @return A translation unit
	 * @throws CoreException
	 */
	static IASTTranslationUnit getTranslationUnit(boolean c_parser, InternalFileContent internalFileContent, IScannerInfo info, IncludeFileContentProvider emptyIncludes, int opts, IParserLogService log) throws CoreException {
		if (c_parser) {
			return GCCLanguage.getDefault().getASTTranslationUnit(internalFileContent, info, emptyIncludes, null, opts, log);
		} else {
			return GPPLanguage.getDefault().getASTTranslationUnit(internalFileContent, info, emptyIncludes, null, opts, log);
		}
	}

	
	static boolean isLoop(IASTNode node, int index) {
		IASTNode[] children = node.getChildren();
		boolean returnFlag;
		
		if (node.getClass().getSimpleName().compareTo("CPPASTForStatement"  ) == 0
		||  node.getClass().getSimpleName().compareTo("CASTForStatement"    ) == 0
		||  node.getClass().getSimpleName().compareTo("CPPASTWhileStatement") == 0
		||  node.getClass().getSimpleName().compareTo("CASTWhileStatement"  ) == 0
		||  node.getClass().getSimpleName().compareTo("CPPASTDoStatement"   ) == 0 
		||  node.getClass().getSimpleName().compareTo("CASTDoStatement"     ) == 0) 
			return true;
		else {
			for (IASTNode iastNode : children)
			{
				returnFlag = isLoop(iastNode, index + 1);
				if (returnFlag)
					return true;
			}
		}
		return false;
	}
	static boolean isLoop(IASTNode node) {
		if (node.getClass().getSimpleName().compareTo("CPPASTForStatement"  ) == 0
				||  node.getClass().getSimpleName().compareTo("CASTForStatement"    ) == 0
				||  node.getClass().getSimpleName().compareTo("CPPASTWhileStatement") == 0
				||  node.getClass().getSimpleName().compareTo("CASTWhileStatement"  ) == 0
				||  node.getClass().getSimpleName().compareTo("CPPASTDoStatement"   ) == 0 
				||  node.getClass().getSimpleName().compareTo("CASTDoStatement"     ) == 0) 
			return true;
		return false;
	}
	static boolean isForStatement(IASTNode node) {
		if (node.getClass().getSimpleName().compareTo("CPPASTForStatement"  ) == 0
		||  node.getClass().getSimpleName().compareTo("CASTForStatement"    ) == 0)
			return true;
		return false;
	}
	
	static boolean isIfStatement(IASTNode node)
	{
		if (node.getClass().getSimpleName().compareTo("CPPASTIfStatement") == 0
		||  node.getClass().getSimpleName().compareTo("CASTIfStatement"  ) == 0)
			return true;
		return false;
	}

	static boolean isExpressionStatement(IASTNode node)
	{
		if (node.getClass().getSimpleName().compareTo("CPPASTExpressionStatement") == 0
		||  node.getClass().getSimpleName().compareTo("CASTExpressionStatement"  ) == 0)
			return true;
		return false;
	}

	static boolean isDeclarationStatement(IASTNode node)
	{
		if (node.getClass().getSimpleName().compareTo("CPPASTDeclarationStatement") == 0
		||  node.getClass().getSimpleName().compareTo("CASTDeclarationStatement"  ) == 0)
			return true;
		return false;
	}

	static boolean isFunctionDefinition(IASTNode node)
	{
		if (node.getClass().getSimpleName().compareTo("CPPASTFunctionDefinition") == 0
		||  node.getClass().getSimpleName().compareTo("CASTFunctionDefinition"  ) == 0)
			return true;
		return false;
	}
	static boolean isFunctionDeclarator(IASTNode node)
	{
		if (node.getClass().getSimpleName().compareTo("CPPASTFunctionDeclarator") == 0
				||  node.getClass().getSimpleName().compareTo("CASTFunctionDeclarator"  ) == 0)
			return true;
		return false;
	}
	
	static boolean isCompoundStatement(IASTNode node)
	{
		if (node.getClass().getSimpleName().compareTo("CPPASTCompoundStatement") == 0
		||  node.getClass().getSimpleName().compareTo("CASTCompoundStatement"  ) == 0)
			return true;
		return false;
	}

	static boolean isSimpleDeclaration(IASTNode node)
	{
		if (node.getClass().getSimpleName().compareTo("CPPASTSimpleDeclaration") == 0
		||  node.getClass().getSimpleName().compareTo("CASTSimpleDeclaration"  ) == 0)
			return true;
		return false;
	}
	
	static boolean isBinaryExpression(IASTNode node)
	{
		if (node.getClass().getSimpleName().compareTo("CPPASTBinaryExpression") == 0
				||  node.getClass().getSimpleName().compareTo("CASTBinaryExpression"  ) == 0)
			return true;
		return false;
	}

	static boolean isArraySubscriptExpression(IASTNode node)
	{
		if (node.getClass().getSimpleName().compareTo("CPPASTArraySubscriptExpression") == 0
				||  node.getClass().getSimpleName().compareTo("CASTArraySubscriptExpression"  ) == 0)
			return true;
		return false;
	}
	
	static boolean isIdExpression(IASTNode node)
	{
		if (node.getClass().getSimpleName().compareTo("CPPASTIdExpression") == 0
		||  node.getClass().getSimpleName().compareTo("CASTIdExpression"  ) == 0)
			return true;
		return false;
	}

	static boolean isLiteralExpression(IASTNode node)
	{
		if (node.getClass().getSimpleName().compareTo("CPPASTLiteralExpression") == 0
		||  node.getClass().getSimpleName().compareTo("CASTLiteralExpression"  ) == 0)
			return true;
		return false;
	}

	static boolean isDeclarator(IASTNode node) {
		if (node.getClass().getSimpleName().compareTo("CPPASTDeclarator") == 0
				||  node.getClass().getSimpleName().compareTo("CASTDeclarator"  ) == 0)
					return true;
				return false;
	}
	
	/**
	 * Prints the help screen to the user, which includes all the flags and their usage
	 */
	static void printHelp()
	{
		System.out.println("Usage: java -jar <JAR> <FILE TO INJECT> [FLAGS]");
		System.out.println("Possible FLAGS:");
		System.out.println("===== Basic Flags =====");
		System.out.println("-sp : Silent Patch - Do not present certain debugging info when running");
		System.out.println("-s  : Silent Mode - Do not present other debugging info when running");
		System.out.println("-k  : Keep Files - Keeps also the C files with each patch already applied (instead of only keeping the .patch files");
		System.out.println("-m  : Solve Macros - Enables algorithm that attempts to solve macro problems");
		System.out.println("-h  : Show this help");
		System.out.println("-e  : Extra faults - Experimental fault injetion for extra operators");
		System.out.println("===== Expert Flags =====");
		System.out.println("-f  : Choose fault - Sets which specific fault will be injected, instead of injecting all faults");
		System.out.println("-c  : C Parser - Uses the GNU C Parser as provided by Eclipse CDT (default)");
		System.out.println("-cpp: C++ Parser - Uses the GNU C++ Parser as provided by Eclipse CDT");
		System.out.println("--no-preprocessing : Do not preprocess (gcc -E) the file before performing the injections. Used when the injector is called as part of a bigger system.");
		System.out.println("--exit-early : Just write the file after being parsed/formatted by Eclipse CDT and do not do any fault injection");
		System.out.println("--ignore-extension : Do not enforce that the input file must end in *.c *.h *.cpp ...");
		
	}
}
