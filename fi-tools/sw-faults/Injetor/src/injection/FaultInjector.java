package injection;

import org.eclipse.cdt.core.dom.ast.*;
import org.eclipse.cdt.core.dom.ast.cpp.ICPPASTVisibilityLabel;
import org.eclipse.cdt.core.dom.rewrite.ASTRewrite;
import org.eclipse.cdt.core.parser.*;
import org.eclipse.cdt.internal.core.dom.parser.c.*;
import org.eclipse.cdt.internal.core.dom.parser.cpp.*;
import org.eclipse.cdt.internal.core.dom.rewrite.ASTModificationStore;
import org.eclipse.cdt.internal.core.dom.rewrite.astwriter.ASTWriter;
import org.eclipse.cdt.internal.core.parser.scanner.InternalFileContent;
import org.eclipse.core.runtime.CoreException;

import java.io.*;
import java.nio.file.*;
import java.util.*;

@SuppressWarnings("Duplicates")
public class FaultInjector {

	private static boolean debug = false;
	private static boolean keep = false;
	private static int faultNumber = -1;
	private static boolean solveMacros = false;
	private static boolean silent = false;
	private static boolean silentpatch = false;
	private static String operator = "";
	private static String content = "";
	private static int targetLine = -1;

	// Holds whether we should use the C parser, or the CPP parser. By default we choose the C parser
	private static boolean c_parser = true;

	// Holds whether we should run the C code over the preprocessor, or we can skip this step (e.g., code has already been preprocessed)
	private static boolean flag_preprocess = true;

	// Holds whether we should terminate early, before any patch is created.
	private static boolean flag_exitEarly = false;

	// Holds whether we should check the extension of the input file or not (.c .h ...)
	private static boolean flag_ignoreExtension = false;

	// Holds whether we should use the extra faults injetion
	private static boolean extraFocus = false;

	// Name of file with code to derivate
	private static String filename = "";
	// Folder with files
	private static String directory = "";

	// Extension of files to derivate
	private static String extension = ".c";
	// Name of file with code formatted
	private static String formattedFile = "";


	/**
	 * Enumeration with all operators and description
	 * @author GonçaloSilva
	 * modified by LuisGonçalo
	 */
	public enum Operators {
		MIFS("MIFS", "_MIFS_", "Missing IF construct and surrounded Statements"),
		MLAC("MLAC", "_MLAC_", "Missing Âand sub-expressionÂ in logical expression used in branch condition"),
		MFC ("MFC" , "_MFC_" , "Missing function call"),
		MIA ("MIA" , "_MIA_" , "Missing IF Around statements"),
		MLOC("MLOC", "_MLOC_", "Missing Âor sub-expressionÂ in logical expression used in branch condition"),
		MLPA("MLPA", "_MLPA_", "Missing Localized Part of the Algorithm"),
		MVAE("MVAE", "_MVAE_", "Missing Variable Assignment with an Expression"),
		MVAV("MVAV", "_MVAV_", "Missing Variable Assignment with a Value"),
		MIEB("MIEB", "_MIEB_", "Missing IF construct plus statements plus else before statements"),
		MVIV("MVIV", "_MVIV_", "Missing Variable initialization with a value"),

		WVAV("WVAV", "_WVAV_", "Wrong Value Assigned to a Variable"),
		WAEP("WAEP", "_WAEP_", "Wrong Arithmetic Expression in a function Parameter"),
		WPFV("WPFV", "_WPFV_", "Wrong Variable in parameter of function Call"),
		WLEC("WLEC", "_WLEC_", "Wrong logical expression used as branch condition"),
		WALR("WALR", "_WALR_", "Wrong algorithm - code was misplaced"),

		EFC( "EFC" , "_EFC_" , "Extraneous function call"),
		EIFS("EIFS", "_EIFS_", "Extra IF construct and surrounded Statements");


		String text;
		String abr;
		String sig;

		Operators(String sig, String abr, String text) {
			this.sig = sig;
			this.abr = abr;
			this.text = text;
		}

		public String getAbr() {
			return abr;
		}
		public String getSig() {
			return sig;
		}

		public String toString() {
			return ordinal() + " -> " + text;
		}
	}

	/**
	 * Main Function
	 * Receive the name of file through the args argument
	 *
	 * @param args name of file to derivate
	 */
	public static void main(String[] args) {
		InternalFileContent internalFileContent = null;

		if (debug) for (int i = 0; i < args.length; i++) {
			System.out.println("args["+i+"] '" +args[i]+"'");
		}

		if (args.length >= 1) {

			for (int i=1; i < args.length; i++) {
//				System.out.println(args[i]+" ");
				switch (args[i]) {
					case "-sp":
						//	 System.out.println("SILENT PATCH OPTION ACTIVATED");
						silentpatch = true;
						break;
					case "-e":
						//	 System.out.println("EXTRA OPTION ACTIVATED");
						extraFocus = true;
						break;
					case "-s":
//		            	 System.out.println("SILENT OPTION ACTIVATED");
						silent = true;
						break;
					case "-d":
//		            	 System.out.println("DEBUG OPTION ACTIVATED");
						debug = true;
						break;
					case "-k":
//		            	 System.out.println("KEEP FILES OPTION ACTIVATED");
						keep = true;
						break;
					case "-h":
						Utilities.printHelp();
						System.exit(0);
						break;
					case "-m":
//		            	 System.out.println("TRY SOLVE MACROS OPTION ACTIVATED");
						solveMacros = true;
						break;
					case "-c":
						c_parser = true;
						break;
					case "-cpp":
						c_parser = false;
						break;
					case "--no-preprocessing":
						flag_preprocess = false;
						break;
					case "--exit-early":
						flag_exitEarly = true;
						break;
					case "--ignore-extension":
						flag_ignoreExtension = true;
						break;
					default:
						if ((args[i].length() == 6 || args[i].length() == 7) && args[i].charAt(1) == 'o' && args[i].charAt(2) == '=') {
							operator = args[i].replaceFirst("-o=", "");
							boolean exist = false;
							for (int j = 0; j < Operators.values().length; j++) {
								if (operator.toUpperCase().compareTo(Operators.values()[j].getSig()) == 0)
								{
									exist = true;
								}
							}
							if (!exist)
								System.out.println("Invalid operator "+operator);
							else
								System.out.println("Operator: "+operator);
						}else if (args[i].length() >= 4 && args[i].charAt(1) == 'f' && args[i].charAt(2) == '=') {
							String faultNumberStr = args[i].replaceFirst("-f=", "");
							faultNumber = Integer.parseInt(faultNumberStr);
						}
						else if (args[i].length() >= 4 && args[i].charAt(1) == 'l' && args[i].charAt(2) == '=') {
							String lineNumberStr = args[i].replaceFirst("-l=", "");
							targetLine = Integer.parseInt(lineNumberStr);
						}
						else {
							// Invalid
							System.out.println("Parameter not recognized: " + args[i]);
						}
						break;
				}
			}
			System.out.println();

			if ((!flag_ignoreExtension) && (!(args[0].endsWith(".c") || args[0].endsWith(".cpp")  || args[0].endsWith(".h"))))
			{
				System.err.println("Argument" + args[0] + " must be a file with extension .c or .cpp");
				System.exit(1);
			}

			directory = "";
			int i = args[0].lastIndexOf('.');
			if (i > 0) {
				extension = args[0].substring(i + 1);
				filename = args[0].substring(0, i);
			}
			System.err.println("Extension " + extension);
			System.err.println("Filename " + filename);

			directory = "";
			FileContent fileContent = FileContent.createForExternalFileLocation(directory + filename + "." + extension);

			try{
				internalFileContent = (InternalFileContent) InternalFileContent.create("code.c", fileContent.toString().toCharArray());
			}catch (NullPointerException e){
				System.err.print("The file doesn't exist.\n");
				System.exit(1);
			}

		}
		else {
			System.err.println("First argument must be a file with extension .c or .cpp");
			Utilities.printHelp();
			System.exit(1);

		}

		// Simple way to be sure if we are using C or CPP parser
		if (debug)
		{
			System.err.println("Variable c_parser is set to " + c_parser);
			if(targetLine != -1)
				System.err.println("Target line is set to " + targetLine);
		}

		@SuppressWarnings("rawtypes")
		Map definedSymbols = new HashMap();
		String[] includePaths = new String[0];
		@SuppressWarnings("unchecked")
		IScannerInfo info = new ScannerInfo(definedSymbols, includePaths);
		IParserLogService log = new DefaultLogService();

		IncludeFileContentProvider emptyIncludes = IncludeFileContentProvider.getEmptyFilesProvider();

		int opts = 8;
		try {

			final IASTTranslationUnit translationUnit = Utilities.getTranslationUnit(c_parser, internalFileContent, info, emptyIncludes, opts, log);

			String code = "";

			code = code.concat(new ASTWriter().write(translationUnit));

			System.out.println();
			System.out.println("FILE: "+filename +"." + extension);

			String backup = "cp "+directory+filename +"." + extension+" "+directory+filename +".origin." + extension;
			ExecuteShellComand obj = new ExecuteShellComand();
			obj.executeCommand(backup, "out.txt");

			if (flag_preprocess) {
				String preprocess = "cp "+directory+filename +"." + extension+" "+directory+filename +".i";
				obj.executeCommand(preprocess, "out.txt");

				// -O0 is the default
				// -fpreprocessed -> Replaced by .i
				String cmd = "gcc -ansi -O0 -dD -E -P "+directory+filename +".i -o "+directory+filename +"." + extension;
				obj.executeCommand(cmd, "out.txt");

				String remove = "rm "+directory+filename +".i";
				obj.executeCommand(remove,"out.txt");
			}


			try {
				content = new String(Files.readAllBytes(Paths.get(directory + filename + "." + extension)));
			} catch (IOException e) {
				e.printStackTrace();
			}

			formattedFile = directory + filename +"." + extension + "._FORMATTED_";
			write(code, new File(formattedFile), "");

			// Check if we should terminate here and dont enter the FI part
			if (flag_exitEarly)
			{
				System.exit(0);
			}

			if (!silent){
				System.out.println("ORIGINAL CODE\n" + code);
			}

			if (!silent) System.out.println("-----------------------------------------------------");

//			Visitor to navigate in tree and apply modifications
			ASTVisitor visitor = new CustomASTVisitor(translationUnit,operator);

			visitor.shouldVisitNames = true;
			visitor.shouldVisitDeclarations = true;
			visitor.shouldVisitDeclarators = false;
			visitor.shouldVisitAttributes = false;
			visitor.shouldVisitStatements = true;
			visitor.shouldVisitTypeIds = false;

			visitor.shouldVisitExpressions = true;
			visitor.shouldVisitImplicitNames = false;

			translationUnit.accept(visitor);

		} catch (CoreException e) {
			e.printStackTrace();
			System.err.println("This shouldn't happen!!!");
			System.exit(1);
//		} catch (ProblemRuntimeException e) {
////			e.printStackTrace();
//			System.err.println("This code compile?");
//			System.exit(1);
		}

	}


	//================================================ Visits - Missing ================================================


	/**
	 * Missing IF construct and surrounded Statements
	 *
	 * @param translationUnit tree of code to modify
	 * @param statement to apply modifications
	 * @param fault number of faults
	 * @return updated number of faults
	 */
	static int mifsVisit(IASTTranslationUnit translationUnit, IASTStatement statement, int fault) {
//		TODO erase the comments
//		Substituir para resolver problema de else ifs
//		if (statement instanceof IASTIfStatement)
		if (statement instanceof IASTIfStatement && !(statement.getParent() instanceof IASTIfStatement))
		{
			for (int i = 0; i < statement.getChildren().length; i++) {
				if (statement.getChildren()[i] instanceof IASTIfStatement)
					return fault;
				if (statement.getChildren()[i] instanceof IASTCompoundStatement)
				{
					for (int j = 0; j < statement.getChildren()[i].getChildren().length; j++) {
						if (statement.getChildren()[i] instanceof IASTIfStatement)
							return fault;
					}
				}
			}
			int countStatements = recursiveCountStatements(statement);
//			System.out.println("statements "+countStatements);
			if (countStatements <= 5) {
				if (debug) {
					System.out.println("-----------------------------------------------------");
					System.out.println("BEGIN " + Operators.MIFS.getAbr() + fault + ": \n\n");
					System.out.println("REMOVE\n" + statement.getRawSignature());
				}
				else if (!silentpatch) System.out.println(Operators.MIFS.getAbr() + fault);

				ASTRewrite r = ASTRewrite.create(translationUnit);
				if (statement.getParent() instanceof IASTForStatement || statement.getParent() instanceof IASTWhileStatement || statement.getParent() instanceof IASTDoStatement){
					if (c_parser)
					{
						r.replace(statement, new CASTNullStatement(), null);
					} else {
						r.replace(statement, new CPPASTNullStatement(), null);
					}
				}
				else
					r.remove(statement, null);

				String code = performModifications(translationUnit, r);

				writeFile(code, fault, Operators.MIFS.getAbr());
				fault += 1;

				if (debug) {
					System.out.println("FINAL CODE\n" + code + "\n\n");
					System.out.println("END");
					System.out.println("-----------------------------------------------------");
				}
			}
		}
		return fault;
	}

	/**
	 * Missing "and sub-expression" in logical expression used as/in branch
	 * condition
	 *
	 * @param translationUnit tree of code to modify
	 * @param node to apply modifications
	 * @param fault number of fault
	 * @return updated number of faults
	 */
	static int mlacVisit(IASTTranslationUnit translationUnit, IASTNode node, int fault) {
//		System.out.println(node.getRawSignature());
		IASTBinaryExpression condition = null;
		if (node instanceof IASTIfStatement) {
			IASTIfStatement newNode = (IASTIfStatement) node;
			if (newNode.getConditionExpression() instanceof IASTBinaryExpression) condition = (IASTBinaryExpression) newNode.getConditionExpression();
		} else if (node instanceof IASTDoStatement) {
			IASTDoStatement newNode = (IASTDoStatement) node;
			if (newNode.getCondition() instanceof IASTBinaryExpression) condition = (IASTBinaryExpression) newNode.getCondition();
		} else if (node instanceof IASTWhileStatement) {
			IASTWhileStatement newNode = (IASTWhileStatement) node;
			if (newNode.getCondition() instanceof IASTBinaryExpression) condition = (IASTBinaryExpression) newNode.getCondition();
		} else
			return fault;

		ArrayList<IASTNode> conditions = recursiveBinaryExpression(condition, new ArrayList<IASTNode>(), IASTBinaryExpression.op_logicalAnd);

		for (int i = 0; i < conditions.size(); i++) {
//			System.out.println("-"+conditions.get(i).getRawSignature());
			if (conditions.get(i) instanceof IASTExpression)
			{
				IASTExpression newExpression = (IASTExpression) conditions.get(i);
				if (newExpression.getParent() instanceof IASTBinaryExpression) {

//					System.out.println(newExpression.getRawSignature());
					IASTBinaryExpression father = (IASTBinaryExpression) newExpression.getParent();

					if (father.getOperator() == IASTBinaryExpression.op_logicalAnd) {
						if (debug){
							System.out.println("-----------------------------------------------------");
							System.out.println("BEGIN " + Operators.MLAC.getAbr() + fault + ": \n\n");
							System.out.println("REMOVE\n" + newExpression.getRawSignature() + "\n");
						}
						else if (!silentpatch) System.out.println(Operators.MLAC.getAbr() + fault);

						ASTRewrite r = ASTRewrite.create(translationUnit);

						if (newExpression == father.getOperand2())
							r.replace(father, father.getOperand1(), null);
						else
							r.replace(father, father.getOperand2(), null);

						String code = performModifications(translationUnit, r);


						writeFile(code, fault, Operators.MLAC.getAbr());
						fault += 1;

						if (debug) {
							System.out.println("FINAL CODE\n" + code + "\n\n");
							System.out.println("END");
							System.out.println("-----------------------------------------------------");
						}
					}
				}
				if (newExpression.getParent() instanceof IASTUnaryExpression) {
					IASTExpression no = (IASTExpression) newExpression.getParent();
//					System.out.println(no.getClass().getName());
					while (!(no instanceof IASTBinaryExpression))
						no = (IASTExpression) no.getParent();
					IASTBinaryExpression father = (IASTBinaryExpression) no;

					if (father.getOperator() == IASTBinaryExpression.op_logicalAnd) {
						if (debug) {
							System.out.println("-----------------------------------------------------");
							System.out.println("BEGIN " + Operators.MLAC.getAbr() + fault + ": \n\n");
							System.out.println("REMOVE\n" + newExpression.getRawSignature() + "\n");
						}
						else if (!silentpatch) System.out.println(Operators.MLAC.getAbr() + fault);

						ASTRewrite r = ASTRewrite.create(translationUnit);

						if (father.getOperand2().contains(newExpression))
							r.replace(father, father.getOperand1(), null);
						else
							r.replace(father, father.getOperand2(), null);

						String code = performModifications(translationUnit, r);


						writeFile(code, fault, Operators.MLAC.getAbr());
						fault += 1;

						if (debug) 	{
							System.out.println("FINAL CODE\n" + code + "\n\n");
							System.out.println("END");
							System.out.println("-----------------------------------------------------");
						}
					}
				}
			}
		}
		return fault;
	}

	/**
	 * Missing function call
	 * @param translationUnit tree of code to modify
	 * @param node to apply modifications
	 * @param fault number of faults
	 * @return updated number of faults
	 */
	static int mfcVisit(IASTTranslationUnit translationUnit, IASTNode node, int fault) {
		if (Utilities.isExpressionStatement(node)) {
			if (debug) {
				System.out.println("-----------------------------------------------------");
				System.out.println("BEGIN " + Operators.MFC.getAbr() + fault + ": \n\n");
				System.out.println("REMOVE\n" + node.getRawSignature() + "\n");
			}
			else if (!silentpatch) System.out.println(Operators.MFC.getAbr() + fault);

			ASTRewrite r = ASTRewrite.create(translationUnit);

			IASTNode[] children = node.getChildren();
			if (children.length == 1) r.remove(children[0], null);
			else r.remove(node, null);

			String code = performModifications(translationUnit, r);
			writeFile(code, fault, Operators.MFC.getAbr());
			fault += 1;

			if (debug) {
				System.out.println("FINAL CODE\n" + code + "\n\n");
				System.out.println("END");
				System.out.println("-----------------------------------------------------");
			}
		}
		return fault;
	}

	/**
	 * Missing IF Around statements
	 *
	 * @param translationUnit tree of code to modify
	 * @param node to apply modifications
	 * @param fault number of faults
	 * @return updated number of faults
	 */
	static int miaVisit(IASTTranslationUnit translationUnit, IASTNode node, int fault) {

//		r.remove(node, null);

		IASTIfStatement newNode = null;
		if (node instanceof IASTIfStatement && !(node.getParent() instanceof IASTIfStatement))
			newNode = (IASTIfStatement) node;

		int countStatements = recursiveCountStatements(node);
//			System.out.println("statements "+countStatements);
		if (newNode != null && countStatements <= 5)
		{
			if (debug)
			{
				System.out.println("-----------------------------------------------------");
				System.out.println("BEGIN " + Operators.MIA.getAbr() + fault + ": \n\n");
				System.out.println("REMOVE\n" + node.getRawSignature());
				System.out.println("INSERT\n" + newNode.getThenClause().getRawSignature());
//				System.out.println("INSERT\n" + node.getChildren()[1].getRawSignature());
			}
			else if (!silentpatch) System.out.println(Operators.MIA.getAbr() + fault);

			ASTRewrite r = ASTRewrite.create(translationUnit);
//			r.insertBefore(node.getParent(), node, newNode.getElseClause(), null);

//			r.insertBefore(node.getParent(), node, newNode.getThenClause(), null);
//			r.insertBefore(node.getParent(), node, node.getChildren()[1], null);
			r.replace(node, node.getChildren()[1], null);

			String code = performModifications(translationUnit, r);
			writeFile(code, fault, Operators.MIA.getAbr());
			fault += 1;
			if (debug)
			{
				System.out.println("FINAL CODE\n" + code + "\n\n");
				System.out.println("END");
				System.out.println("-----------------------------------------------------");
			}
		}
		return fault;
	}

	/**
	 * Missing "or sub-expression" in logical expression used as/in branch condition
	 *
	 * @param translationUnit tree of code to modify
	 * @param node to apply modifications
	 * @param fault number of fault
	 * @return updated number of faults
	 */
	static int mlocVisit(IASTTranslationUnit translationUnit, IASTNode node, int fault) {

//		System.out.println(node.getRawSignature());
		IASTBinaryExpression condition = null;
		if (node instanceof IASTIfStatement) {
			IASTIfStatement newNode = (IASTIfStatement) node;
			if (newNode.getConditionExpression() instanceof IASTBinaryExpression) condition = (IASTBinaryExpression) newNode.getConditionExpression();
		} else if (node instanceof IASTDoStatement) {
			IASTDoStatement newNode = (IASTDoStatement) node;
			if (newNode.getCondition() instanceof IASTBinaryExpression) condition = (IASTBinaryExpression) newNode.getCondition();
		} else if (node instanceof IASTWhileStatement) {
			IASTWhileStatement newNode = (IASTWhileStatement) node;
			if (newNode.getCondition() instanceof IASTBinaryExpression) condition = (IASTBinaryExpression) newNode.getCondition();
		} else
			return fault;

		ArrayList<IASTNode> conditions = recursiveBinaryExpression(condition, new ArrayList<IASTNode>(), IASTBinaryExpression.op_logicalOr);
//		System.out.println(conditions.size());

//		conditions.forEach(element -> System.out.println(element.getRawSignature()));

		for (int i = 0; i < conditions.size(); i++) {
//			System.out.println("-"+conditions.get(i).getRawSignature());
			if (conditions.get(i) instanceof IASTExpression)
			{
				IASTExpression newExpression = (IASTExpression) conditions.get(i);
				if (newExpression.getParent() instanceof IASTBinaryExpression) {

//					System.out.println(newExpression.getRawSignature());
					IASTBinaryExpression father = (IASTBinaryExpression) newExpression.getParent();

					if (father.getOperator() == IASTBinaryExpression.op_logicalOr) {
						if (debug) {
							System.out.println("-----------------------------------------------------");
							System.out.println("BEGIN " + Operators.MLOC.getAbr() + fault + ": \n\n");
							System.out.println("REMOVE\n" + newExpression.getRawSignature() + "\n");
						}
						else if (!silentpatch) System.out.println(Operators.MLOC.getAbr() + fault);

						ASTRewrite r = ASTRewrite.create(translationUnit);

						if (newExpression == father.getOperand2())
							r.replace(father, father.getOperand1(), null);
						else
							r.replace(father, father.getOperand2(), null);

						String code = performModifications(translationUnit, r);


						writeFile(code, fault, Operators.MLOC.getAbr());
						fault += 1;

						if (debug) {
							System.out.println("FINAL CODE\n" + code + "\n\n");
							System.out.println("END");
							System.out.println("-----------------------------------------------------");
						}
					}
				}
				if (newExpression.getParent() instanceof IASTUnaryExpression) {
					IASTExpression no = (IASTExpression) newExpression.getParent();
					while (!(no instanceof IASTBinaryExpression))
						no = (IASTExpression) no.getParent();
					IASTBinaryExpression father = (IASTBinaryExpression) no;

					if (father.getOperator() == IASTBinaryExpression.op_logicalOr) {
						if (debug) {
							System.out.println("-----------------------------------------------------");
							System.out.println("BEGIN " + Operators.MLOC.getAbr() + fault + ": \n\n");
							System.out.println("REMOVE\n" + newExpression.getRawSignature() + "\n");
						}
						else if (!silentpatch) System.out.println(Operators.MLOC.getAbr() + fault);

						ASTRewrite r = ASTRewrite.create(translationUnit);

						if (father.getOperand2().contains(newExpression))
							r.replace(father, father.getOperand1(), null);
						else
							r.replace(father, father.getOperand2(), null);

						String code = performModifications(translationUnit, r);


						writeFile(code, fault, Operators.MLOC.getAbr());
						fault += 1;

						if (debug){
							System.out.println("FINAL CODE\n" + code + "\n\n");
							System.out.println("END");
							System.out.println("-----------------------------------------------------");
						}
					}
				}
			}
		}
		return fault;
	}

	/**
	 * Missing small and localized part of the algorithm
	 *
	 * @param translationUnit tree of code to modify
	 * @param statement which contains all the code to apply modifications
	 * @param fault number of fault
	 * @return updated number of faults
	 */
	static int mlpaVisit(IASTTranslationUnit translationUnit, IASTStatement statement, int fault) {
		if (statement instanceof IASTCompoundStatement) {
			ArrayList<IASTNode> toRemove = new ArrayList<>();
			IASTNode[] children = statement.getChildren();
			//System.out.println("numero de filhos "+children.length);
			for (IASTNode child : children) {
				if (child instanceof IASTExpressionStatement && !Utilities.isLoop(child) && !Utilities.isIfStatement(child)) {
					toRemove.add(child);
				}
			}
			if (toRemove.size() > 1) {
				for (int i = 2; i <= toRemove.size(); i++) {
					for (int j = 0; j < toRemove.size(); j++) {
						ASTRewrite r = ASTRewrite.create(translationUnit);
						int counter = 0;
						String output = "";
						// k = 0 1 | k = 1 2 | k = 0 1 2
						for (int k = j; k < j+i && k < toRemove.size() && j+i <= toRemove.size(); k++) {
							if (k == j) {
								if (debug)
									output = "-----------------------------------------------------\nBEGIN " + Operators.MLPA.getAbr() + fault + ": \n\nREMOVE\n";
								else if (!silentpatch) output = Operators.MLPA.getAbr() + fault+ "\n";
							}
							if (debug)
								output += toRemove.get(k).getRawSignature()+"\n";
							r.remove(toRemove.get(k), null);
							counter++;

//							constraints c02 e c10

							if (!(counter > 5 || counter == toRemove.get(k).getParent().getChildren().length) && (k+1 >= j+i || k+1 >= toRemove.size() || j+i > toRemove.size()))							{
								System.out.print(output);
								String code = performModifications(translationUnit, r);
								writeFile(code, fault, Operators.MLPA.getAbr());
								fault += 1;

								if (debug) {
									System.out.println("FINAL CODE\n" + code + "\n\n");
									System.out.println("END");
									System.out.println("-----------------------------------------------------");
								}
							}
						}
					}
				}
			}
		}
		return fault;
	}

	/**
	 * Missing variable assignment using an expression
	 *
	 * @param translationUnit tree of code to modify
	 * @param statement to apply modifications
	 * @param fault number of fault
	 * @return updated number of faults
	 */
	static int mvaeVisit(IASTTranslationUnit translationUnit, IASTStatement statement, int fault) {
//		var
		if (Utilities.isExpressionStatement(statement))
		{
			IASTNode[] children = statement.getChildren();
//			c02
			int count = recursiveCountStatements(statement.getParent());
//			System.out.println("contador "+count);
			if (count > 1 && children != null && children.length == 1 && Utilities.isBinaryExpression(children[0]))
			{
				IASTNode[] grandchildren = null;
				//if (children[0] instanceof CASTBinaryExpression) grandchildren = ((CASTBinaryExpression) children[0]).getChildren();
				if (children[0] instanceof IASTBinaryExpression) grandchildren = children[0].getChildren();
				if (grandchildren != null && grandchildren.length == 2)
				{
					if ((Utilities.isIdExpression(grandchildren[0]) && Utilities.isBinaryExpression(grandchildren[1])) || (Utilities.isIdExpression(grandchildren[0]) && Utilities.isIdExpression(grandchildren[1])))
					{

						if (debug) {
							System.out.println("-----------------------------------------------------");
							System.out.println("BEGIN " + Operators.MVAE.getAbr() + fault + ": \n\n");
							System.out.println("REMOVE\n"+statement.getRawSignature());
						}
						else if (!silentpatch) System.out.println(Operators.MVAE.getAbr() + fault);
						ASTRewrite r = ASTRewrite.create(translationUnit);
						r.remove(statement, null);

						String code = performModifications(translationUnit, r);
						writeFile(code, fault, Operators.MVAE.getAbr());
						fault += 1;

						if (debug) {
							System.out.println("FINAL CODE\n" + code + "\n\n");
							System.out.println("END");
							System.out.println("-----------------------------------------------------");
						}
					}
				}
			}
		}
		return fault;
	}

	/**
	 * Missing variable assignment using a value
	 *
	 * @param translationUnit tree of code to modify
	 * @param statement to apply modifications
	 * @param fault number of fault
	 * @return updated number of faults
	 */
	static int mvavVisit(IASTTranslationUnit translationUnit, IASTStatement statement, int fault) {
//		var
		if ((Utilities.isExpressionStatement(statement) && !(statement.getParent() instanceof IASTIfStatement) && statement.getParent().getChildren().length >= 2)
				|| (Utilities.isExpressionStatement(statement) && statement.getParent() instanceof IASTIfStatement && statement.getParent().getChildren().length > 3))
		{
			IASTNode[] children = statement.getChildren();
			if (children != null && children.length == 1 && Utilities.isBinaryExpression(children[0]))
			{
				IASTNode[] grandchildren = children[0].getChildren();

//				if (grandchildren != null && grandchildren.length == 2)
				if ((grandchildren != null && grandchildren.length == 2 && !(children[0] instanceof IASTIfStatement))
						|| (grandchildren != null && grandchildren.length > 2 && children[0] instanceof IASTIfStatement))
				{
					if (Utilities.isIdExpression(grandchildren[0]) && (Utilities.isIdExpression(grandchildren[1]) || Utilities.isLiteralExpression(grandchildren[1])) )
					{
						try {
							Integer.parseInt(grandchildren[1].getRawSignature());
							if (debug) {
								System.out.println("-----------------------------------------------------");
								System.out.println("BEGIN " + Operators.MVAV.getAbr() + fault + ": \n\n");
								System.out.println("REMOVE\n"+statement.getRawSignature());
							}
							else if (!silentpatch) System.out.println(Operators.MVAV.getAbr() + fault);
							ASTRewrite r = ASTRewrite.create(translationUnit);

							r.remove(statement, null);

							String code = performModifications(translationUnit, r);
							writeFile(code, fault, Operators.MVAV.getAbr());
							fault += 1;

							if (debug) {
								System.out.println("FINAL CODE\n" + code + "\n\n");
								System.out.println("END");
								System.out.println("-----------------------------------------------------");
							}
						} catch (NumberFormatException e) {
							return fault;
						}
					}
				}
			}
		}
		return fault;
	}

	/**
	 * Missing IF construct plus statements plus else before statements
	 *
	 * @param translationUnit tree of code to modify
	 * @param node to apply modifications
	 * @param fault number of faults
	 * @return updated number of faults
	 */
	static int miebVisit(IASTTranslationUnit translationUnit, IASTNode node, int fault) {
		ASTRewrite r = ASTRewrite.create(translationUnit);
//		r.remove(node, null);

		IASTIfStatement newNode = null;
		if (node instanceof IASTIfStatement && node.getChildren().length >= 3 && !(node.getParent() instanceof IASTIfStatement))
		{
			for (int i = 0; i < node.getChildren().length; i++) {
				if (node.getChildren()[i] instanceof IASTIfStatement)
					return fault;
			}
			newNode = (IASTIfStatement) node;
		}

		if (newNode != null && newNode.getElseClause() != null) {
			if (debug)
			{
				System.out.println("-----------------------------------------------------");
				System.out.println("BEGIN " + Operators.MIEB.getAbr() + fault + ": \n\n");
				System.out.println("MODIFY\n" + node.getRawSignature());
			}
			else if (!silentpatch) System.out.println(Operators.MIEB.getAbr() + fault);

			r.replace(node, newNode.getElseClause(), null);
//			r.insertBefore(node.getParent(), node, newNode.getElseClause(), null);

//			TODO Fix problem!!!
//
//			System.out.println("newNode2 "+node.getChildren()[2].getRawSignature());
//			System.exit(0);
//			r.insertBefore(parent, insertionPoint, newNode, editGroup)
//			r.insertBefore(node, node, node.getChildren()[2], null);
//			r.replace(node, ((IASTIfStatement)node.getChildren()[2])., null);
//			r.replace(node.getParent(), node, null);

			try{
				String code = performModifications(translationUnit, r);
				writeFile(code, fault, Operators.MIEB.getAbr());
				fault += 1;
				if (debug)
				{
					System.out.println("FINAL CODE\n" + code + "\n\n");
					System.out.println("END");
					System.out.println("-----------------------------------------------------");
				}
			}catch (NullPointerException e){

				System.out.println(node);
				System.out.println(newNode.getElseClause());
				System.out.println("insertionPoint "+node.getRawSignature());
				System.out.println("newNode "+newNode.getElseClause().getRawSignature());

			}
		}
		return fault;
	}

	/**
	 * Missing variable initialization
	 *
	 * @param translationUnit tree of code to modify
	 * @param statement to apply modifications
	 * @param fault number of faults
	 * @return updated number of faults
	 */
	static int mvivVisit(IASTTranslationUnit translationUnit, IASTStatement statement, int fault) {
//		int var = value
		if (Utilities.isDeclarationStatement(statement))
		{
			IASTNode[] children = statement.getChildren();
			IASTNode[] grandchildren = children[0].getChildren();
			if (grandchildren[1] != null && grandchildren[1].getChildren().length == 2)
			{
				IASTNode[] grandgrandchildren = grandchildren[1].getChildren();
//				System.out.println(statement.getRawSignature());
				if (grandgrandchildren[1] instanceof IASTEqualsInitializer && grandgrandchildren[1].getChildren()[0] instanceof IASTLiteralExpression)
				{
					if (debug) {
						System.out.println("-----------------------------------------------------");
						System.out.println("BEGIN " + Operators.MVIV.getAbr() + fault + ": \n\n");
					}
					else if (!silentpatch) System.out.println(Operators.MVIV.getAbr() + fault);
					ASTRewrite r = ASTRewrite.create(translationUnit);

					IASTLiteralExpression toChange = (IASTLiteralExpression) grandgrandchildren[1].getChildren()[0];

					IASTName astName;
					IASTIdExpression toAdd;

					if (c_parser)
					{
						astName = new CASTName(grandgrandchildren[0].getRawSignature().toCharArray());
						toAdd = new CASTIdExpression(astName);
					} else {
						astName = new CPPASTName(grandgrandchildren[0].getRawSignature().toCharArray());
						toAdd = new CPPASTIdExpression(astName);
					}
					r.replace(toChange, toAdd, null);

					String code = performModifications(translationUnit, r);
					writeFile(code, fault, Operators.MVIV.getAbr());
					fault += 1;

					if (debug) {
						System.out.println("REPLACE\n"+toChange.getRawSignature());
						System.out.println("TO\n"+ grandgrandchildren[0].getRawSignature());
						System.out.println("FINAL CODE\n" + code + "\n\n");
						System.out.println("END");
						System.out.println("-----------------------------------------------------");
					}
				}
			}
			return fault;
		}

//		caso nao tenha sido feita a inicializao inicialmente!!!
//		var = value
		if (Utilities.isExpressionStatement(statement))
		{
			IASTNode[] children = statement.getChildren();
			if (children != null && children.length == 1 && Utilities.isBinaryExpression(children[0]))
			{
				IASTNode[] grandchildren = null;
				//if (children[0] instanceof CASTBinaryExpression) grandchildren = ((CASTBinaryExpression) children[0]).getChildren();
				if (children[0] instanceof IASTBinaryExpression) grandchildren = children[0].getChildren();
				if (grandchildren != null && grandchildren.length == 2)
				{
					if (Utilities.isIdExpression(grandchildren[0]) && Utilities.isLiteralExpression(grandchildren[1]))
					{
						if (debug) {
							System.out.println("-----------------------------------------------------");
							System.out.println("BEGIN " + Operators.MVIV.getAbr() + fault + ": \n\n");
							System.out.println("REMOVE\n"+grandchildren[1].getRawSignature());
						}
						else if (!silentpatch) System.out.println(Operators.MVIV.getAbr() + fault);
						ASTRewrite r = ASTRewrite.create(translationUnit);

						IASTName astName;
						IASTIdExpression toAdd;

						if (c_parser)
						{
							astName = new CASTName(grandchildren[0].getChildren()[0].getRawSignature().toCharArray());
							toAdd = new CASTIdExpression(astName);
						} else {
							astName = new CPPASTName(grandchildren[0].getChildren()[0].getRawSignature().toCharArray());
							toAdd = new CPPASTIdExpression(astName);
						}

						r.replace(grandchildren[1], toAdd, null);

						String code = performModifications(translationUnit, r);
						writeFile(code, fault, Operators.MVIV.getAbr());
						fault += 1;

						if (debug) {
							System.out.println("FINAL CODE\n" + code + "\n\n");
							System.out.println("END");
							System.out.println("-----------------------------------------------------");
						}
					}
				}
			}
		}
		return fault;
	}


	//================================================= Visits - Wrong =================================================


	/**
	 * Wrong value assigned to variable
	 *
	 * @param translationUnit tree of code to modify
	 * @param statement to apply modifications
	 * @param fault number of faults
	 * @return updated number of faults
	 */
	static int wvavVisit(IASTTranslationUnit translationUnit, IASTStatement statement, int fault) {
//		var = value
		if (Utilities.isExpressionStatement(statement))
		{
			IASTNode[] children = statement.getChildren();
			if (children != null && children.length == 1 && Utilities.isBinaryExpression(children[0]))
			{
				IASTNode[] grandchildren = null;
				//if (children[0] instanceof CASTBinaryExpression) grandchildren = ((CASTBinaryExpression) children[0]).getChildren();
				if (children[0] instanceof IASTBinaryExpression) grandchildren = children[0].getChildren();
				if (grandchildren != null && grandchildren.length == 2)
				{
					if (Utilities.isIdExpression(grandchildren[0]) && Utilities.isLiteralExpression(grandchildren[1]))
					{

						try {
							IASTLiteralExpression op1, op2;
							IASTBinaryExpression toAdd;

							if (c_parser) {
								op1 = new CASTLiteralExpression(Integer.parseInt(grandchildren[1].getRawSignature()), "0".toCharArray());
								op2 = new CASTLiteralExpression(255, "0xff".toCharArray());
								toAdd = new CASTBinaryExpression(IASTBinaryExpression.op_binaryXor,op1,op2);
							} else {
								op1 = new CPPASTLiteralExpression(Integer.parseInt(grandchildren[1].getRawSignature()), "0".toCharArray());
								op2 = new CPPASTLiteralExpression(255, "0xff".toCharArray());
								toAdd = new CPPASTBinaryExpression(IASTBinaryExpression.op_binaryXor,op1,op2);
							}


							if (debug) {
								System.out.println("-----------------------------------------------------");
								System.out.println("BEGIN " + Operators.WVAV.getAbr() + fault + ": \n\n");
								System.out.println("REMOVE\n"+children[0].getRawSignature());
							}
							else if (!silentpatch) System.out.println(Operators.WVAV.getAbr() + fault);
							ASTRewrite r = ASTRewrite.create(translationUnit);


							r.replace(grandchildren[1], toAdd, null);

							String code = performModifications(translationUnit, r);
							writeFile(code, fault, Operators.WVAV.getAbr());
							fault += 1;

							if (debug) {
								System.out.println("FINAL CODE\n" + code + "\n\n");
								System.out.println("END");
								System.out.println("-----------------------------------------------------");
							}
						} catch(NumberFormatException e){
//							System.out.println("NÃ£o fez o parse!!!");
//							System.out.println(children[0]);
//							System.out.println(grandchildren[1].getRawSignature());
						}
					}
				}
			}
		}
		return fault;
	}

	/**
	 * Wrong arithmetic expression in parameter of function call
	 *
	 * @param translationUnit tree of code to modify
	 * @param statement to apply modifications
	 * @param fault number of faults
	 * @return updated number of faults
	 */
	static int waepVisit(IASTTranslationUnit translationUnit, IASTStatement statement, int fault) {
		if (statement instanceof IASTExpressionStatement)
		{
			IASTNode[] children = statement.getChildren();
			if (children != null && children.length == 1 && children[0] instanceof IASTFunctionCallExpression)
			{
//				find function call
//				if have parameters
				if (children[0].getChildren().length > 1)
				{
					IASTNode[] grandChildren = children[0].getChildren();
//					LiteralExpression
//					IdExpression

					for (int i = 0; i < grandChildren.length; i++) {
//					BinaryExpression
						if ((Utilities.isBinaryExpression(grandChildren[i]) || Utilities.isArraySubscriptExpression(grandChildren[i])) && grandChildren[i].getChildren().length > 1)
						{
							for (int j = 0; j < grandChildren[i].getChildren().length; j++) {
								if (debug) {
									System.out.println("-----------------------------------------------------");
									System.out.println("BEGIN " + Operators.WAEP.getAbr() + fault + ": \n\n");
									System.out.println("CHANGE\n"+grandChildren[i].getRawSignature());
									System.out.println("TO\n"+grandChildren[i].getChildren()[j].getRawSignature());
								}
								else if (!silentpatch) System.out.println(Operators.WAEP.getAbr() + fault);

								ASTRewrite r = ASTRewrite.create(translationUnit);

								r.replace(grandChildren[i], grandChildren[i].getChildren()[j], null);

								String code = performModifications(translationUnit, r);
								writeFile(code, fault, Operators.WAEP.getAbr());
								fault += 1;

								if (debug){
									System.out.println("FINAL CODE\n" + code + "\n\n");
									System.out.println("END");
									System.out.println("-----------------------------------------------------");
								}
							}
						}
					}
				}
			}
		}
		return fault;
	}

	/**
	 * Wrong variable used in parameter of function call
	 *
	 * @param translationUnit tree of code to modify
	 * @param statement to apply modifications
	 * @param fault number of faults
	 * @return updated number of faults
	 */
	static int wpfvVisit(IASTTranslationUnit translationUnit, IASTStatement statement, int fault, ArrayList<IASTNode> vars) {
//		verificar tipos das variaveis
//		substituir variavel original
		if (statement instanceof IASTExpressionStatement)
		{
			IASTNode[] children = statement.getChildren();
//			var = func();
//			ExpressionStatement
//			BinaryExpression
//			IdExpression FunctionCallExpression

			if (children != null && children.length >= 1 && children[0] instanceof IASTBinaryExpression)
			{
				IASTNode[] grandChildren = children[0].getChildren();
				if (grandChildren != null && grandChildren.length >= 2 && grandChildren[1] instanceof IASTFunctionCallExpression && grandChildren[1].getChildren().length > 1)
				{
					for (int i = 1; i < grandChildren[1].getChildren().length; i++) {
						ArrayList<String> variaveis = searchVar(grandChildren[1].getChildren()[i], vars);

						for (int j = 0; j < variaveis.size(); j++) {
							if (debug)
							{
								System.out.println("-----------------------------------------------------");
								System.out.println("BEGIN " + Operators.WPFV.getAbr() + fault + ": \n\n");
								System.out.println("CHANGE\n"+grandChildren[1].getChildren()[i].getRawSignature());
							}
							else
							{
								if (!silentpatch) System.out.println(Operators.WPFV.getAbr() + fault);
							}
							ASTRewrite r = ASTRewrite.create(translationUnit);


							IASTName astName;
							IASTIdExpression toAdd;

							if (c_parser)
							{
								astName = new CASTName(variaveis.get(j).toCharArray());
								toAdd = new CASTIdExpression(astName);
							} else {
								astName = new CPPASTName(variaveis.get(j).toCharArray());
								toAdd = new CPPASTIdExpression(astName);
							}

							r.replace(grandChildren[1].getChildren()[i], toAdd, null);

							String code = performModifications(translationUnit, r);
							writeFile(code, fault, Operators.WPFV.getAbr());
							fault += 1;

							if (debug)
							{
								System.out.println("FINAL CODE\n" + code + "\n\n");
								System.out.println("END");
								System.out.println("-----------------------------------------------------");
							}
						}
					}
				}
			}

//			func();
//			ExpressionStatement
//			FunctionCallExpression
			if (children != null && children.length >= 1 && children[0] instanceof IASTFunctionCallExpression  && children[0].getChildren().length > 1)
			{
				for (int i = 1; i < children[0].getChildren().length; i++) {
					ArrayList<String> variaveis = searchVar(children[0].getChildren()[i], vars);

					for (int j = 0; j < variaveis.size(); j++) {
						if (debug)
						{
							System.out.println("-----------------------------------------------------");
							System.out.println("BEGIN " + Operators.WPFV.getAbr() + fault + ": \n\n");
							System.out.println("CHANGE\n"+children[0].getChildren()[i].getRawSignature());
						}
						else if (!silentpatch) System.out.println(Operators.WPFV.getAbr() + fault);
						ASTRewrite r = ASTRewrite.create(translationUnit);

						IASTName astName;
						IASTIdExpression toAdd;

						if (c_parser)
						{
							astName = new CASTName(variaveis.get(j).toCharArray());
							toAdd = new CASTIdExpression(astName);
						} else {
							astName = new CPPASTName(variaveis.get(j).toCharArray());
							toAdd = new CPPASTIdExpression(astName);
						}

						r.replace(children[0].getChildren()[i], toAdd, null);

						String code = performModifications(translationUnit, r);
						writeFile(code, fault, Operators.WPFV.getAbr());
						fault += 1;

						if (debug)
						{
							System.out.println("FINAL CODE\n" + code + "\n\n");
							System.out.println("END");
							System.out.println("-----------------------------------------------------");
						}
					}
				}
			}
		}
		return fault;
	}

	/**
	 * Wrong logical expression used as branch condition
	 *
	 * @param translationUnit tree of code to modify
	 * @param statement to apply modifications
	 * @param fault number of faults
	 * @return updated number of faults
	 */
	static int wlecVisit(IASTTranslationUnit translationUnit, IASTStatement statement, int fault) {
		//Find if, for, while and do-while conditions
		IASTNode children;
		if (statement instanceof IASTIfStatement || statement instanceof IASTWhileStatement) {
			children = statement.getChildren()[0];
		} else if (statement instanceof IASTForStatement || statement instanceof IASTDoStatement) {
			children = statement.getChildren()[1];
		} else
			return fault;


		if (children instanceof IASTBinaryExpression) {
			//Limitation - AST don't support various binary expressions
			IASTIdExpression op1;
			IASTExpression op2;
			op1 = (IASTIdExpression) children.getChildren()[0].copy();

			// Differenciate (i<a) from (i<10)
			if (children.getChildren()[1] instanceof IASTIdExpression){
				op2 = (IASTIdExpression) children.getChildren()[1].copy();
			}else if(children.getChildren()[1] instanceof IASTLiteralExpression) {
				op2 = (IASTLiteralExpression) children.getChildren()[1].copy();
			}else{
				return fault;
			}
			String newCode = children.getChildren()[0].getRawSignature() + " ";
			IASTBinaryExpression node;

			//Change the logical operator to the most common
			if(children.getRawSignature().contains("&&")){
				node = new CASTBinaryExpression(IASTBinaryExpression.op_logicalOr,op1,op2);
				newCode += "|| " + children.getChildren()[1].getRawSignature();
			}
			else if(children.getRawSignature().contains("||")){
				node = new CASTBinaryExpression(IASTBinaryExpression.op_logicalAnd,op1,op2);
				newCode += "&& " + children.getChildren()[1].getRawSignature();
			}
			else if(children.getRawSignature().contains("<=")){
				node = new CASTBinaryExpression(IASTBinaryExpression.op_lessThan,op1,op2);
				newCode += "< " + children.getChildren()[1].getRawSignature();
			}
			else if(children.getRawSignature().contains(">=")){
				node = new CASTBinaryExpression(IASTBinaryExpression.op_greaterThan,op1,op2);
				newCode += "> " + children.getChildren()[1].getRawSignature();
			}
			else if(children.getRawSignature().contains("<")){
				node = new CASTBinaryExpression(IASTBinaryExpression.op_greaterThan,op1,op2);
				newCode += "> " + children.getChildren()[1].getRawSignature();
			}
			else if(children.getRawSignature().contains(">")){
				node = new CASTBinaryExpression(IASTBinaryExpression.op_lessThan,op1,op2);
				newCode += "< " + children.getChildren()[1].getRawSignature();
			}
			else if(children.getRawSignature().contains("==")){
				node = new CASTBinaryExpression(IASTBinaryExpression.op_notequals,op1,op2);
				newCode += "!= " + children.getChildren()[1].getRawSignature();
			}
			else if(children.getRawSignature().contains("!=")){
				node = new CASTBinaryExpression(IASTBinaryExpression.op_equals,op1,op2);
				newCode += "== " + children.getChildren()[1].getRawSignature();
			}else{
				return fault;
			}

			if (debug) {
				System.out.println("-----------------------------------------------------");
				System.out.println("BEGIN " + Operators.WLEC.getAbr() + fault + ": \n\n");
				System.out.println("CHANGE\n"+children.getRawSignature());
				System.out.println("TO\n"+newCode);
			}
			else if (!silentpatch) System.out.println(Operators.WLEC.getAbr() + fault);

			ASTRewrite r = ASTRewrite.create(translationUnit);

			r.replace(children, node, null);

			String code = performModifications(translationUnit, r);
			writeFile(code, fault, Operators.WLEC.getAbr());
			fault++;

			if (debug){
				System.out.println("FINAL CODE\n" + code + "\n\n");
				System.out.println("END");
				System.out.println("-----------------------------------------------------");
			}
		}
		return fault;
	}

	/**
	 * Wrong algorithm - code was misplaced
	 *
	 * @param translationUnit tree of code to modify
	 * @param statement to apply modifications
	 * @param fault number of faults
	 * @return updated number of faults
	 */
	static int walrVisit(IASTTranslationUnit translationUnit, IASTStatement statement, int fault) {
		if (statement instanceof IASTCompoundStatement) {
			ArrayList<IASTNode> toRemove = new ArrayList<>();
			IASTNode[] children = statement.getChildren();
			for (IASTNode child : children) {
				if (child instanceof IASTExpressionStatement && !Utilities.isLoop(child) && !Utilities.isIfStatement(child)) {
					toRemove.add(child);
				}
			}

			if (toRemove.size() > 1) {
				for (int i = 2; i < toRemove.size(); i++) {
					for (int j = 0; j < toRemove.size(); j++) {
						ASTRewrite r = ASTRewrite.create(translationUnit);
						ArrayList<IASTNode> toAdd = new ArrayList<>();
						int counter = 0;
						String output = "";
						int newPlace;

						// k = 0 1 | k = 1 2 | k = 0 1 2
						for (int k = j; k < j+i && k < toRemove.size() && j+i <= toRemove.size(); k++) {
							if (k == j) {
								if (debug)
									output = "-----------------------------------------------------\nBEGIN " + Operators.WALR.getAbr() + fault + ": \n\nCHANGE\n";
								else if (!silentpatch)
									output = Operators.WALR.getAbr() + fault+ "\n";


							}
							if (debug)
								output += toRemove.get(k).getRawSignature()+"\n";


							toAdd.add(toRemove.get(k));
							r.remove(toRemove.get(k), null);
							counter++;

							if (!(counter > 5 || counter == toRemove.get(k).getParent().getChildren().length) && (k+1 >= j+i || k+1 >= toRemove.size() || j+i > toRemove.size())){
								// Finds a new place, always downwards, if it reachs the bottom start from the beginning
								int oldPlace = newPlace = Arrays.asList(children).indexOf(toAdd.get(0));

								while(newPlace == oldPlace){
									newPlace = (newPlace + 2) % children.length;
								}

								for (IASTNode node : toAdd){
									r.insertBefore(children[newPlace].getParent(), children[newPlace], node.copy(), null);
								}

								System.out.print(output);
								String code = performModifications(translationUnit, r);
								writeFile(code, fault, Operators.WALR.getAbr());
								fault += 1;

								if (debug)
								{
									System.out.println("END");
									System.out.println("-----------------------------------------------------");
								}
							}
						}
					}
				}
			}
		}
		return fault;
	}


	//================================================= Visits - Extra =================================================

	// TODO - It's not totally deterministic
	/**
	 * Extraneous function call
	 *
	 * @param translationUnit tree of code to modify
	 * @param statement to apply modifications
	 * @param fault number of faults
	 * @return updated number of faults
	 */
	static int efcVisit(IASTTranslationUnit translationUnit, IASTStatement statement, int fault, ArrayList<IASTNode> functions,ArrayList<IASTNode> vars, String currentFunctionName) {
		boolean callable = true;
		Random randomGenerator = new Random(0);

		// Same function verification and prevent empty diff
		IASTNode function;
		if(functions.size() > 1 && !(statement instanceof IASTCompoundStatement)){
			// Get callable function name
			function = functions.get(randomGenerator.nextInt(functions.size()));
			while(function.getChildren()[0].getRawSignature().equals(currentFunctionName)){
				// Get another function if it chooses itself
				function = functions.get(randomGenerator.nextInt(functions.size()));
			}
		} else {
			return fault;
		}

		// Create a string with the code to print
		StringBuilder newCode = new StringBuilder();
		if(debug) {
			newCode = new StringBuilder(function.getChildren()[0].getRawSignature() + "(");
		}

		List<IASTIdExpression> parameters = new ArrayList<>();

		// Get all the parameters
		for (int i=1;i<function.getChildren().length;i++) {
			String parameterType = function.getChildren()[i].getRawSignature().split(" ")[0];
			boolean usable = false;
			// Verify if there are variables to use
			if(!vars.isEmpty()) {
				int start = randomGenerator.nextInt(vars.size());
				// Start from a random variable to avoid repetition
				for (int j = 0; j < vars.size(); j++) {
					int index = (start + j) % vars.size();
					String variableType = vars.get(index).getRawSignature().split(" ")[0];
					// Variable is suitable for parameter, if one is usable, stop
					if (variableType.equals(parameterType)) {
						usable = true;
						// Get the variable name
						IASTIdExpression argument = new CASTIdExpression((IASTName) vars.get(index).getChildren()[1].getChildren()[0].copy());
						parameters.add(argument);

						// Add variables to string
						if(debug) {
							newCode.append(vars.get(index).getChildren()[1].getChildren()[0]).append(",");
						}
						break;
					}
				}
			}
			// There is at least one variable suitable for the parameter
			if(!usable){
				callable = false;
			}
		}

		// Remove last comma
		if(debug) {
			if (newCode.toString().endsWith(",")) {
				newCode = new StringBuilder(newCode.substring(0, newCode.length() - 1));
			}
			newCode.append(");");
		}


		// There are variables for all the parameters
		if(callable){
			if (debug) {
				System.out.println("-----------------------------------------------------");
				System.out.println("BEGIN " + Operators.EFC.getAbr() + fault + ": \n\n");
				System.out.println("ADD\n"+ newCode.toString());
			}
			else if (!silentpatch)
				System.out.println(Operators.EFC.getAbr() + fault);

			// Create the nodes
			IASTExpressionStatement stat = new CASTExpressionStatement();
			IASTFunctionCallExpression callNode = new CASTFunctionCallExpression();

			// Get the function name
			IASTExpression functionName = new CASTIdExpression((IASTName)function.getChildren()[0].copy());
			callNode.setFunctionNameExpression(functionName);

			// Get the function arguments
			IASTIdExpression[] functionArgs = new IASTIdExpression[parameters.size()];
			functionArgs = parameters.toArray(functionArgs);
			callNode.setArguments(functionArgs);

			// Function call as ExpressionStatement's child
			stat.setExpression(callNode);

			ASTRewrite r = ASTRewrite.create(translationUnit);

			r.insertBefore(statement.getParent(),statement,stat,null);

			String code = performModifications(translationUnit, r);
			writeFile(code, fault, Operators.EFC.getAbr());
			fault++;

			if (debug) {
				System.out.println("FINAL CODE\n" + code + "\n\n");
				System.out.println("END");
				System.out.println("-----------------------------------------------------");
			}
		}

		return fault;
	}

	/**
	 * Extraneous IF construct and surrounded Statements
	 *
	 * @param translationUnit tree of code to modify
	 * @param statement to apply modifications
	 * @param fault number of faults
	 * @return updated number of faults
	 */
	static int eifsVisit(IASTTranslationUnit translationUnit, IASTStatement statement, int fault, ArrayList<IASTNode> ids,ArrayList<IASTNode> vars, String functionName) {
		if (!(statement instanceof IASTCompoundStatement)){
			String toAdd;

			toAdd = "if";
			// Create branch condition and create the node
			IASTIdExpression op1, op2;

			op1 = new CASTIdExpression((IASTName) (ids.get(getPopularElement(ids)[0]).copy()));
			op2 = new CASTIdExpression((IASTName) (ids.get(getPopularElement(ids)[1]).copy()));

			// TODO - Change the condition
			IASTBinaryExpression condition = new CASTBinaryExpression(IASTBinaryExpression.op_equals,op1,op2);
			toAdd += "(" + op1.getName() + " == " + op2.getName() + "){\n";

			// Create the compound node, with all the statements inside the if
			IASTCompoundStatement compound = new CASTCompoundStatement();

			IASTExpression ifExpr;
			//((CASTIdExpression) ifExpr).setName(new CASTName("teste".toCharArray()));
			// TODO - Add statements

			// Por cada statement adicionar mais uma linha
			//toAdd += statement_text "\n";

			//compound.addStatement(new CASTExpressionStatement(ifExpr));

			// Create the if node and add the condition and compound
			IASTIfStatement ifNode = new CASTIfStatement();
			ifNode.setConditionExpression(condition);
			ifNode.setThenClause(compound);

			toAdd += "\n}";

			if (debug) {
				System.out.println("-----------------------------------------------------");
				System.out.println("BEGIN " + Operators.EIFS.getAbr() + fault + ": \n\n");
				System.out.println("ADD\n"+ toAdd);
			}
			else if (!silentpatch)
				System.out.println(Operators.EIFS.getAbr() + fault);

			ASTRewrite r = ASTRewrite.create(translationUnit);

			r.insertBefore(statement.getParent(),statement,ifNode,null);

			String code = performModifications(translationUnit, r);
			writeFile(code, fault, Operators.EIFS.getAbr());
			fault++;

			if (debug) {
				System.out.println("FINAL CODE\n" + code + "\n\n");
				System.out.println("END");
				System.out.println("-----------------------------------------------------");
			}
		}

		return fault;
	}


	//==================================================== Utilities ====================================================


	//	WORKAROUND TO SOLVE MACROS
	private static String solveMacros(String code, String content, int i, int j, String type) {
		boolean debugging = false;
//		System.out.println(code);
//		int i = 0;
//		int j = 0;
//		content.replaceAll("(\\\\r)?\\\\n", System.getProperty("line.separator"));
		boolean macrosproblem = true;
		if (type.compareTo("") == 0) macrosproblem = false;

		while(i < content.length() && j <= code.length()) {

//			se tiver macros no fim do codigo
			if (j == code.length()) {
				if (debugging) System.out.println("A");
				char add = content.charAt(i);
				String begin = code.substring(0,j);
				String end = code.substring(j);
				code = begin + add + end;
				j++;
				i++;
			}
//			se os chars forem iguais
			else if (content.charAt(i) == code.charAt(j)){
				if (debugging) System.out.println("B");
				j++;
				i++;
			}
			else if (j+1 < code.length() && content.charAt(i) == code.charAt(j+1) && i+1 < content.length() && content.charAt(i+1) == ' ' && code.charAt(j) == ' '){
				if (debugging) System.out.println("C");
				j+=2;
				i+=2;
			}
			else if (i+1 < content.length() && content.charAt(i+1) == code.charAt(j) && j+1 < code.length() && content.charAt(i) == ' ' && code.charAt(j+1) == ' '){
				if (debugging) System.out.println("D");
				j+=2;
				i+=2;
			}
			else if (i+1 < content.length() && content.charAt(i) == ' ' && code.charAt(j) == content.charAt(i+1)){
				if (debugging) System.out.println("E");
				j++;
				i+=2;
			}
			else if (code.charAt(j) == '\n' || code.charAt(j) == ' ' || code.charAt(j) == '\r' || code.charAt(j) == '\t') {
				if (debugging) System.out.println("F");
				j++;
			}
			else if (content.charAt(i) == '\n' || content.charAt(i) == ' ' || content.charAt(i) == '\r' || content.charAt(i) == '\t') {
				if (debugging) System.out.println("G");
				i++;
			}
			else if (content.charAt(i) == '"') {
				if (debugging) System.out.println("H");
				int auxi = i+1;
				while(content.charAt(auxi) == '\n' || content.charAt(auxi) == ' ' || content.charAt(auxi) == '\t'){
					auxi++;
				}
				if (content.charAt(auxi) == '"')
					i = auxi+1;
				else i++;
			}
			else if (content.charAt(i) == '#'){
				if (debugging) System.out.println("I");
				int auxi = i+1;
				while((content.charAt(auxi) != '\n' || content.charAt(auxi-1) == '\\' ) && auxi+1 < content.length()){
					auxi++;
				}
				boolean flag = false;
				while(content.charAt(i+1) == 'i' && content.charAt(i+2) == 'f' && ((content.charAt(i+3) == ' ' && content.charAt(i+4) != '!') || content.charAt(i+3) == 'd') && !(content.charAt(auxi) == '#' && content.charAt(auxi+1) == 'e')){
//						&& content.charAt(auxi+2) != 'n' && content.charAt(auxi+3) != 'd'
					auxi++;
					flag = true;
//					System.out.println("apanha o else");
//					System.out.println(content.charAt(auxi));
//					System.out.println(content.charAt(auxi+1));
//					System.out.println(content.charAt(i+1) == 'i');
//					System.out.println(content.charAt(i+2) == 'f');
//					System.out.println((content.charAt(i+3) == ' ' || content.charAt(i+3) == 'd'));
//					System.out.println(content.charAt(auxi) != '#');
//					System.out.println(content.charAt(auxi+1) != 'e');
				}
				while(flag && content.charAt(auxi) != '\n' && auxi+1 < content.length()){
					auxi++;
				}
//				char add = content.charAt(i);
				String add = content.substring(i, auxi+1);
//				System.out.println(add);
				String begin = code.substring(0,j);
				String end = code.substring(j);
				code = begin + add + end;
				j += add.length();
				i += add.length();

				if (!macrosproblem)
				{
					System.out.println("Verify Limitations with macros!");
					macrosproblem = true;
				}
//				j++;
//				i++;
			}
			else {
				if (debugging) System.out.println("J");
				if (type.compareTo(Operators.MVIV.getAbr()) == 0 || type.compareTo(Operators.WVAV.getAbr()) == 0){
					while(i < content.length() && content.charAt(i) != ';')	i++;
					while(j < code.length() && code.charAt(j) != ';')	j++;
				}
//				else if (type.compareTo(Operators.MIA.getAbr()) == 0){
//					while(i < content.length() && (content.charAt(i) != '{' || content.charAt(i) != ')'))	i++;
//
//					if (i < content.length()){
//						while(j < code.length() && code.charAt(j) != content.charAt(i))	j++;
//					}
//					else {
//						while(j < code.length() && (code.charAt(j) != '{' || code.charAt(j) != ')'))	j++;
//					}
//				}
				else if (type.compareTo(Operators.WPFV.getAbr()) == 0){
					while(i < content.length() && content.charAt(i) != ',')	i++;
					while(j < code.length() && code.charAt(j) != ',')	j++;
				}
				else {
					if (debugging) System.out.print(content.charAt(i));
					i++;
				}
//				formattedFile = directory + filename +"." + extension + "._FORMATTED_";
//				write(code, new File(formattedFile));
//				File file = new File(formattedFile);
//				try {
//					if (!file.exists()) {
//						file.createNewFile();
//					}
//					PrintWriter out;
//					out = new PrintWriter(new BufferedWriter(new FileWriter(file)));
//					out.write(code);
//					out.close();
//				} catch (IOException e) {
//					e.printStackTrace();
//				}
//				System.out.println("1'"+content.substring(i-20,i+20)+"'");
//				System.out.println("2'"+code.substring(j-20,j+20)+"'");
//				System.out.println("f");
//				System.exit(0);
			}
		}
		return code;
	}

	@SuppressWarnings("unused")
	static void printVars(ArrayList<IASTSimpleDeclaration> vars) {
		for (IASTSimpleDeclaration var : vars) {
			try {
				System.out.println("type: " + var.getSyntax());
			} catch (ExpansionOverlapsBoundaryException e) {
				e.printStackTrace();
			}
			IASTDeclarator[] declarators = var.getDeclarators();
			for (IASTDeclarator iastDeclarator : declarators) {
				System.out.println("var:  " + iastDeclarator.getName());
				if (iastDeclarator.getInitializer() != null) {
					System.out.println("init: " + iastDeclarator.getInitializer().getRawSignature());
				}
			}
		}
	}

	private static String searchVarType(IASTNode var, ArrayList<IASTNode> vars) {
		for (int i = 0; i < vars.size(); i++) {
//			FunctionDeclarator
			if (vars.get(i) instanceof IASTFunctionDeclarator)
			{
				IASTNode[] children = vars.get(i).getChildren();
				if (children != null && children.length >= 2 && children[1] instanceof IASTParameterDeclaration)
//				Name ParameterDeclaration
				{
					IASTNode[] grandChildren = children[1].getChildren();
					if (grandChildren != null && grandChildren.length >= 2 && grandChildren[0] instanceof IASTSimpleDeclSpecifier)
					{
//					SimpleDeclSpecifier Declarator
						for (int j = 1; j < grandChildren.length; j++) {
							if (grandChildren[j] instanceof IASTDeclarator && grandChildren[j].getRawSignature().compareTo(var.getRawSignature()) == 0)
							{
								return grandChildren[0].getRawSignature();
							}
						}
					}
				}
			}
//			SimpleDeclaration
			else if ((vars.get(i) instanceof IASTSimpleDeclaration) || (vars.get(i) instanceof IASTParameterDeclaration))
			{
				IASTNode[] children = vars.get(i).getChildren();

				if (children.length >= 2 && children[0] instanceof IASTSimpleDeclSpecifier)
//				SimpleDeclSpecifier Declarator
//				type name
				{
					for (int j = 1; j < children.length; j++) {
						if (children[j] instanceof IASTDeclarator && children[j].getRawSignature().compareTo(var.getRawSignature()) == 0)
						{
							return children[0].getRawSignature();
						}
					}
				}
			}
		}
		return "";
	}

	private static ArrayList<String> searchVar(IASTNode var, ArrayList<IASTNode> vars) {
//		procurar tipo da variavel
//		encontrar outra do mesmo tipo e que seja diferente da inicial
//		retorna-la
		ArrayList<String> variaveis = new ArrayList<>();

//		TODO cases with explicit strings in code ?
		String type = searchVarType(var, vars);

		for (int i = 0; i < vars.size(); i++) {
//			DeclarationStatement
			if (vars.get(i) instanceof IASTDeclarationStatement && var != null && var.getChildren().length >= 1)
			{
				return searchVar(var.getChildren()[0], vars);
			}
//			FunctionDeclarator
			else if (vars.get(i) instanceof IASTFunctionDeclarator)
			{
				IASTNode[] children = vars.get(i).getChildren();
				if (children != null && children.length >= 2 && children[1] instanceof IASTParameterDeclaration)
//				Name ParameterDeclaration
				{
					IASTNode[] grandChildren = children[1].getChildren();
					if (grandChildren != null && grandChildren.length >= 2 && grandChildren[0] instanceof IASTSimpleDeclSpecifier && grandChildren[0].getRawSignature().compareTo(type) == 0 && grandChildren[1] instanceof IASTDeclarator && grandChildren[1].getRawSignature().compareTo(var.getRawSignature()) != 0)
//					SimpleDeclSpecifier Declarator
					{
						variaveis.add(grandChildren[1].getRawSignature());
					}
				}
			}
//			SimpleDeclaration
			else if ((vars.get(i) instanceof IASTSimpleDeclaration) || (vars.get(i) instanceof IASTParameterDeclaration))
			{
				IASTNode[] children = vars.get(i).getChildren();
				if (children.length >= 2 && children[0] instanceof IASTSimpleDeclSpecifier && children[0].getRawSignature().compareTo(type) == 0)
//				SimpleDeclSpecifier Declarator
//				type name
				{
					for (int j = 1; j < children.length; j++) {
						if (children[j] instanceof IASTDeclarator && children[j].getRawSignature().compareTo(var.getRawSignature()) != 0)
						{
							variaveis.add(children[j].getChildren()[0].getRawSignature());
						}
					}
				}
			}
		}
		return variaveis;
	}

	/**
	 * Get modifications from ASTRewrite r given by parameter
	 * Then apply in ASTWriter to get the code
	 * Write the code with modifications to file
	 *
	 * @param translationUnit tree of code to modify
	 * @param r modifications to apply in translationUnit
	 * @return modified code
	 */
	private static String performModifications(IASTTranslationUnit translationUnit, ASTRewrite r) {
		String code = "";
		try {
			ASTModificationStore newModificationStore = r.getfModificationStore();

			ASTWriter writer = new ASTWriter();
			writer.setModificationStore(newModificationStore);
			code = code.concat(writer.write(translationUnit));
		} catch (SecurityException | IllegalArgumentException e2) {
			e2.printStackTrace();
		}
		return code;
	}

	/**
	 * Write file with code and generate the patch
	 * @param code to be written
	 * @param fault number of fault
	 * @param type of fault
	 */
	private static void writeFile(String code, int fault, String type) {
		if ((faultNumber != -1) && (fault >= faultNumber)) return;

		String outputFile = directory + filename + "." + extension + "." + type + fault;

		write(code, new File(outputFile), type);
		Path path = Paths.get(outputFile);


		String label = filename + "." + extension;
		String cmd = "diff --strip-trailing-cr -U 3 -p -b -B --label "+label +" "+ formattedFile + " " + outputFile;
//		String cmd = "diff -U 0 --ignore-blank-lines --minimal --label "+label +" "+ formattedFile + " " + outputFile;
		ExecuteShellComand obj = new ExecuteShellComand();

		String replace = "cp "+formattedFile+" "+directory+label;
		obj.executeCommand(replace, "out.txt");

		obj.executeCommand(cmd, outputFile + ".patch");

//		System.out.println();
		if (!keep)
		{
			try {
				Files.delete(path);
			} catch (NoSuchFileException x) {
				System.err.format("%s: no such" + " file or directory%n", path);
			} catch (DirectoryNotEmptyException x) {
				System.err.format("%s not empty%n", path);
			} catch (IOException x) {
				// File permission problems are caught here.
				x.printStackTrace();
			}
		}
	}

	private static int recursiveCountStatements(IASTNode node) {
		int counter = 0;
		for (int i = 0; i < node.getChildren().length; i++) {
			if (node.getChildren()[i] instanceof IASTStatement && !(node.getChildren()[i] instanceof IASTCompoundStatement) && (node instanceof IASTIfStatement))
			{

			}
			else if (node.getChildren()[i] instanceof IASTStatement && !(node.getChildren()[i] instanceof IASTCompoundStatement) && !(node.getChildren()[i] instanceof IASTIfStatement))
			{
				counter += recursiveCountStatements(node.getChildren()[i]);
				counter++;
			}
		}
		return counter;
	}

	private static ArrayList<IASTNode> recursiveBinaryExpression(IASTNode node, ArrayList<IASTNode> conditions, int op) {

		if (node != null)
		{
			IASTNode[] conditionChildren = node.getChildren();
			if (conditionChildren != null && conditionChildren.length >= 1)
			{
				for (int i = 0; i < conditionChildren.length && (conditionChildren[i] instanceof IASTBinaryExpression || conditionChildren[i] instanceof IASTUnaryExpression); i++) {
//					System.out.println("chama para "+conditionChildren[i].getRawSignature());
					conditions = recursiveBinaryExpression(conditionChildren[i], conditions, op);
				}
			}
			if (conditionChildren != null
					&&conditionChildren.length >= 2
					&& (conditionChildren[0] instanceof IASTIdExpression
					|| conditionChildren[0] instanceof IASTFunctionCallExpression
					|| conditionChildren[0] instanceof IASTFieldReference
					|| conditionChildren[0] instanceof IASTArraySubscriptExpression
					|| (conditionChildren[0] instanceof IASTBinaryExpression
			)
//									&& !(conditionChildren[1] instanceof IASTBinaryExpression))
					&& ((IASTBinaryExpression)node).getOperator()!= op)
//							)
					&& (conditionChildren[1] instanceof IASTLiteralExpression
					|| conditionChildren[1] instanceof IASTUnaryExpression
					|| conditionChildren[1] instanceof IASTIdExpression
					|| conditionChildren[1] instanceof IASTArraySubscriptExpression
					|| (conditionChildren[1] instanceof IASTBinaryExpression)
//									&& !(conditionChildren[0] instanceof IASTBinaryExpression))
					&& ((IASTBinaryExpression)node).getOperator()!= op)
//							)
			) {
//				if (conditionChildren != null && conditionChildren.length >= 2 && (conditionChildren[0] instanceof IASTIdExpression || conditionChildren[0] instanceof IASTFunctionCallExpression) && conditionChildren[1] instanceof IASTLiteralExpression) {
//				System.out.println("Adiciona "+node.getRawSignature());
				conditions.add(node);
				return conditions;
//			} else if (conditionChildren != null
//					&&conditionChildren.length >= 2 &&
//
//					|| ){
//				conditions.add(node);
//				return conditions;
			}
//			else {

//				if (conditionChildren != null && conditionChildren.length >= 2)
//				{
//					recursiveBinaryExpression(conditionChildren[1], conditions);
//				}
//			}
		}
		return conditions;
	}

	@SuppressWarnings("unused")
	private static void printTree(IASTNode node, int index) {
		IASTNode[] children = node.getChildren();

		boolean printContents = true;

		if ((node instanceof IASTTranslationUnit)) {
			printContents = false;
		}

		String offset = "";
		try {
			offset = node.getSyntax() != null ? " (offset: " + node.getFileLocation().getNodeOffset() + "," + node.getFileLocation().getNodeLength() + ")" : "";
			printContents = node.getFileLocation().getNodeLength() < 30;
		} catch (ExpansionOverlapsBoundaryException e) {
			e.printStackTrace();
		} catch (UnsupportedOperationException e) {
			offset = "UnsupportedOperationException";
		}

		if (node.getRawSignature().contains("\n"))
			System.out.println(String.format("%1$" + index * 2 + "s", "-") + node.getClass().getSimpleName() + offset + " -> "
					+ (printContents ? node.getRawSignature().replaceAll("\n", " \\ ") : node.getRawSignature().subSequence(0, node.getRawSignature().indexOf("\n"))));
		else
			System.out.println(String.format("%1$" + index * 2 + "s", "-") + node.getClass().getSimpleName() + offset + " -> "
					+ (printContents ? node.getRawSignature().replaceAll("\n", " \\ ") : node.getRawSignature().subSequence(0, 5)));

		for (IASTNode iastNode : children)
			printTree(iastNode, index + 1);
	}

	@SuppressWarnings("unused")
	public static boolean isVisible(IASTNode current) {
		IASTNode declator = current.getParent().getParent();
		IASTNode[] children = declator.getChildren();

		for (IASTNode iastNode : children) {
			if ((iastNode instanceof ICPPASTVisibilityLabel)) {
				return 1 == ((ICPPASTVisibilityLabel) iastNode).getVisibility();
			}
		}
		return false;
	}

	private static void write(String code, File file, String type) {
		// create file if it necessary

		if (solveMacros) code = solveMacros(code, content, 0, 0, type);

		try {
			if (!file.exists()) {
				file.createNewFile();
			}
			// write to file
			// Use printwriter with buffered writer is faster than FileWriter
			PrintWriter out;

			out = new PrintWriter(new BufferedWriter(new FileWriter(file)));

			out.write(code);

			out.close();

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	static int getTargetLine() {
		return targetLine;
	}

	private static int[] getPopularElement(ArrayList<IASTNode> ids) {
		int count = 1, tempCount;
		int popular = 0;
		int oldPopular = 0;
		IASTNode temp;
		for (int i = 0; i < ids.size()-1; i++) {
			temp = ids.get(i);
			tempCount = 0;
			for (int j = 1; j < ids.size(); j++) {
				if (temp.getRawSignature().equals(ids.get(j).getRawSignature()))
					tempCount++;
			}
			if (tempCount > count) {
				oldPopular = popular;
				popular = i;
				count = tempCount;
			}
		}
		// Prevent same id
		if(ids.get(oldPopular).getRawSignature().equals(ids.get(popular).getRawSignature())){
			oldPopular = (oldPopular + 1) % ids.size();
		}
		return new int[]{oldPopular,popular};
	}

	public static boolean isExtraFocus() {
		return extraFocus;
	}
}
