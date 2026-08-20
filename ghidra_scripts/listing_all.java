/* ### BEGIN ###
#category Export
#name listing_all
#description Export raw Listing (bytes, disassembly) - compatible version
### END ### */

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.MemoryAccessException;
import ghidra.program.model.mem.MemoryBlock;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

public class listing_all extends GhidraScript {

    @Override
    public void run() throws Exception {
        String outputPath = System.getProperty("user.home") + "\\Desktop\\listing_output.txt";
        
        File outputFile = new File(outputPath);
        File parentDir = outputFile.getParentFile();
        if (parentDir != null && !parentDir.exists()) {
            if (!parentDir.mkdirs()) {
                println("ERROR: Cannot create directory");
                return;
            }
        }

        println("Exporting listing to: " + outputPath);

        try (PrintWriter writer = new PrintWriter(new FileWriter(outputFile))) {
            Listing listing = currentProgram.getListing();
            long totalBytes = 0;
            long instructionCount = 0;
            
            writer.println("================================================");
            writer.println("Program: " + currentProgram.getName());
            writer.println("================================================");
            writer.println();

            MemoryBlock[] blocks = currentProgram.getMemory().getBlocks();
            
            for (MemoryBlock block : blocks) {
                if (!block.isInitialized()) {
                    continue;
                }
                
                // Пропускаем блоки с нулевым размером
                if (block.getSize() <= 0) {
                    continue;
                }

                writer.println("--- Block: " + block.getName() + " ---");
                writer.println("  Start: " + block.getStart() + "  Size: " + block.getSize() + " bytes");
                writer.println();

                Address current = block.getStart();
                Address end = block.getEnd();

                long bytesInBlock = 0;
                
                while (current != null && current.compareTo(end) <= 0 && !monitor.isCancelled()) {
                    try {
                        byte[] bytes = new byte[16];
                        int bytesRead = currentProgram.getMemory().getBytes(current, bytes, 0, 16);
                        
                        if (bytesRead <= 0) {
                            current = current.add(1);
                            continue;
                        }

                        String hexBytes = bytesToHex(bytes, bytesRead);
                        
                        Instruction instr = listing.getInstructionAt(current);
                        String disassembly = "";
                        
                        if (instr != null) {
                            disassembly = instr.toString();
                            instructionCount++;
                            current = current.add(instr.getLength());
                        } else {
                            Data data = listing.getDefinedDataAt(current);
                            if (data != null) {
                                disassembly = data.getDataType().getName() + " " + data.getDefaultValueRepresentation();
                            } else {
                                disassembly = "??";
                            }
                            current = current.add(bytesRead);
                        }
                        
                        totalBytes += bytesRead;
                        bytesInBlock += bytesRead;

                        writer.println(String.format("%-12s %-23s %s",
                            current.toString(),
                            hexBytes,
                            disassembly
                        ));

                    } catch (MemoryAccessException e) {
                        writer.println(current.toString() + "  ??  [unreadable]");
                        current = current.add(1);
                    }
                }

                writer.println();
                writer.println("--- End of block: " + block.getName() + " (" + bytesInBlock + " bytes) ---");
                writer.println();
            }

            writer.println("================================================");
            writer.println("Total bytes: " + totalBytes);
            writer.println("Instructions: " + instructionCount);
            writer.println("================================================");

            println("Exported " + totalBytes + " bytes, " + instructionCount + " instructions");
        }

        println("Output: " + outputPath);
    }

    private String bytesToHex(byte[] bytes, int count) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < count && i < bytes.length; i++) {
            sb.append(String.format("%02X ", bytes[i] & 0xFF));
        }
        return sb.toString().trim();
    }
}
