package injection;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;

class StreamGobbler extends Thread {
	InputStream is;
	String type;
	String file;

	StreamGobbler(InputStream is, String type, String file) {
		this.is = is;
		this.type = type;
		if (file != null)
			this.file = file;
	}

	public void run() {

		String output = "";
		try {
			InputStreamReader isr = new InputStreamReader(is);
			BufferedReader br = new BufferedReader(isr);
			String line = null;
			while ((line = br.readLine()) != null){
//				System.out.println(type + ">" + line);
				output = output.concat(line+"\n");
			}
		} catch (IOException ioe) {
			ioe.printStackTrace();
		} 
		if (file != null) write(output, new File(file));
	}
	
	public static void write(String content, File file) {
		// create file if it necessary
		try {
			if (!file.exists()) {
				file.createNewFile();
			}
			// write to file
			// Use printwriter with buffered writer is faster than FileWriter
			PrintWriter out;

			out = new PrintWriter(new BufferedWriter(new FileWriter(file)));

			out.write(content);

			out.close();

		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}

class ExecuteShellComand {

	String executeCommand(String command, String file) {
//		System.out.println("RUNNING COMMAND: \n>> " + command);
		StringBuffer output = new StringBuffer();

		Process proc;
		try {
			proc = Runtime.getRuntime().exec(command);
			
			String s;
			BufferedReader br = new BufferedReader(new InputStreamReader(proc.getInputStream()));
//			while ((s = br.readLine()) != null) {
//				System.out.println(s);
//			}
			
//			 any error message?
          StreamGobbler errorGobbler = new StreamGobbler(proc.getErrorStream(), "ERROR", null);            
          
          // any output?
          StreamGobbler outputGobbler = new StreamGobbler(proc.getInputStream(), "OUTPUT", file);
              
          // kick them off
          errorGobbler.start();
          outputGobbler.start();
                                  
          // any error???
          proc.waitFor();
          int exitVal = proc.exitValue();
//          System.out.println("exitValue"+exitVal);
//          if (exitVal != 0){
//        	  System.out.println("ExitValue: " + exitVal);
        	  String[] result = command.split(" ");
        	  if (exitVal != 0 && result[0].compareTo("gcc") == 0){
        		  System.out.println("ERROR RUNNING COMMAND: \n>> " + command);
        		  System.out.println("gcc is correctly installed? use you gcc at backend?");        		  
        		  System.exit(1);
        	  }
        	  else if (exitVal > 1 && result[0].compareTo("diff") == 0){
        		  System.out.println("ERROR RUNNING COMMAND: \n>> " + command);
        		  System.out.println("diff is correctly installed?");
        		  System.exit(1);
        	  }
        	  else if (exitVal > 0 && result[0].compareTo("cp") == 0){
        		  System.out.println("ERROR RUNNING COMMAND: \n>> " + command);
        		  System.out.println("problems with cp command");
        		  System.exit(1);
        	  }
//          }

		} catch (Exception e) {
			e.printStackTrace();
		}

		return output.toString();

	}

}