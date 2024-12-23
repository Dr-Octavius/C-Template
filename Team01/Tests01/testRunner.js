const fs = require('fs');
const { spawn } = require('child_process');
const path = require('path');

const makeFileDirs = ['cmake-build-debug', 'build', 'cmake-build'];
const rootDir = path.join(__dirname, '..');
const resultBaseDir = path.join(rootDir, 'Tests01/TestOutFiles');
const sourceBaseDir = path.join(rootDir, 'Tests01/TestFiles');
const xslFilePath = path.join(rootDir, 'Tests01/TestOutFiles', 'analysis.xsl'); // Corrected path to the XSL file

// Ensure the result directory exists
if (!fs.existsSync(resultBaseDir)) {
    fs.mkdirSync(resultBaseDir, { recursive: true });
}

// Helper function to find the autotester executable
function findAutotesterPath() {
    for (const dir of makeFileDirs) {
        const autotesterPath = path.join(rootDir, 'Code01', dir, 'src/autotester/autotester');
        if (fs.existsSync(autotesterPath) || fs.existsSync(`${autotesterPath}.exe`)) {
            return autotesterPath;
        }
    }
    throw new Error('Autotester executable not found in any configured directory.');
}

let autotesterPath;
try {
    autotesterPath = findAutotesterPath();
} catch (error) {
    console.error(error.message);
    process.exit(1);
}

function processDirectory(sourceDir, resultDir) {
    fs.readdir(sourceDir, { withFileTypes: true }, (err, entries) => {
        if (err) {
            console.error('Error listing test files:', err);
            return;
        }

        entries.forEach(entry => {
            if (entry.isDirectory()) {
                const newSourceDir = path.join(sourceDir, entry.name);
                const newResultDir = path.join(resultDir, entry.name);
                if (!fs.existsSync(newResultDir)) {
                    fs.mkdirSync(newResultDir, { recursive: true });
                }
                processDirectory(newSourceDir, newResultDir);
            } else if (entry.name.endsWith('_source.txt')) {
                const baseName = entry.name.replace('_source.txt', '');
                const sourceFile = path.join(sourceDir, `${baseName}_source.txt`);
                const queryFile = path.join(sourceDir, `${baseName}_queries.txt`);
                const outputFile = path.join(resultDir, `${baseName}_out.xml`);

                const autotesterProcess = spawn(autotesterPath, [sourceFile, queryFile, outputFile]);
                autotesterProcess.on('error', error => console.error(`Error with test ${baseName}:`, error));
                autotesterProcess.stdout.on('data', data => {});
                autotesterProcess.stderr.on('data', data => {});
                autotesterProcess.on('exit', (code, signal) => {
                    if (code !== null) {
                        console.log(`Test ${baseName} exited with code: ${code}`);
                    } else if (signal !== null) {
                        console.log(`Test ${baseName} terminated by signal: ${signal}`);
                    }
                });
                autotesterProcess.on('close', code => {
                    console.log(`Test ${baseName} completed with code ${code}`);
                    if (code === 0) {
                        if (fs.existsSync(xslFilePath)) {
                            fs.copyFileSync(xslFilePath, path.join(resultDir, 'analysis.xsl'));
                        } else {
                            console.error('Missing XSL file:', xslFilePath);
                        }
                    }
                });
            }
        });
    });
}

// Start processing from the base directory
processDirectory(sourceBaseDir, resultBaseDir);
