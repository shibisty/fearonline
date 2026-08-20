//@category Decompiler

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.util.task.ConsoleTaskMonitor;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;

public class decompile_all extends GhidraScript {

    @Override
    public void run() throws Exception {

        String outputPath = System.getProperty("user.home") + "\\Desktop\\decompile_output.c";

        File outputFile = new File(outputPath);

        PrintWriter writer = new PrintWriter(
            new FileWriter(outputFile)
        );

        DecompInterface decompiler = new DecompInterface();

        decompiler.openProgram(currentProgram);

        ConsoleTaskMonitor monitor =
            new ConsoleTaskMonitor();

        FunctionIterator functions =
            currentProgram
                .getFunctionManager()
                .getFunctions(true);

        int count = 0;

        try {

            while (functions.hasNext()) {

                if (monitor.isCancelled()) {
                    break;
                }

                Function function =
                    functions.next();

                DecompileResults result =
                    decompiler.decompileFunction(
                        function,
                        60,
                        monitor
                    );

                if (!result.decompileCompleted()) {
                    continue;
                }

                if (result.getDecompiledFunction() == null) {
                    continue;
                }

                writer.println();
                writer.println(
                    "/* ================================================== */"
                );

                writer.println(
                    "/* Function: " +
                    function.getName() +
                    " */"
                );

                writer.println(
                    "/* Address: " +
                    function.getEntryPoint() +
                    " */"
                );

                writer.println(
                    "/* ================================================== */"
                );

                writer.println();

                writer.println(
                    result
                        .getDecompiledFunction()
                        .getC()
                );

                writer.println();

                count++;
            }

        }
        finally {

            writer.close();

            decompiler.dispose();
        }

        println(
            "=========================================="
        );

        println(
            "Decompiled functions: " +
            count
        );

        println(
            "Output: " +
            outputPath
        );

        println(
            "=========================================="
        );
    }
}
