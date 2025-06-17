#include <Arduino.h>
#include "Arduino_UnifiedStorage.h"

// Select your storage type here:
InternalStorage storage; // or SDStorage / USBStorage

void printFileInfo(UFile& file) {
  Serial.println("---- File Info ----");
  Serial.print("Path: ");
  Serial.println(file.getPath());

  long pos = file.tell();
  Serial.print("Position (tell): ");
  Serial.println(pos);

  size_t avail = file.available();
  Serial.print("Available bytes: ");
  Serial.println(avail);
}

void testFlushAndTell(UFile& file) {
  Serial.println("\n[TEST] Writing to file and flushing...");

  const String content = "Flush and tell test\nSecond line\n";
  file.write(content);
  file.flush();

  Serial.println("[TEST] After flush:");
  printFileInfo(file);
}

void testAvailableAndSeek(UFile& file) {
  file.seek(0);
  Serial.println("\n[TEST] Reading file content with tell() and available():");

  while (file.available()) {
    Serial.print("[tell: ");
    Serial.print(file.tell());
    Serial.print("] ");

    char ch = file.read();
    Serial.write(ch);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Arduino_UnifiedStorage::debuggingModeEnabled = true;

  if (!storage.begin()) {
    Serial.println("[ERROR] Failed to mount storage.");
    return;
  }

  Folder root = storage.getRootFolder();
  Folder testFolder = root.createSubfolder("flush_test");

  // Remove file if it already exists

  // Create and write
  UFile file = testFolder.createFile("demo.txt", FileMode::WRITE);
  testFlushAndTell(file);
  file.close();

  // Reopen in read mode
  file.changeMode(FileMode::READ);
  printFileInfo(file);
  testAvailableAndSeek(file);

  // Clean up
  file.close();
  file.remove();
  Serial.println("[INFO] File test completed and removed.");
}

void loop() {
  // Nothing here
}
