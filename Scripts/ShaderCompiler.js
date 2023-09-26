const path = require('path');
const fs = require('fs');
const child_process = require('child_process');

/*
    Argument list
    -dxlevel: 9, 11, 12 | DirectX version.
    -olevel 0, 1, 2, 3  | Optimization level for DX9 and DX11.
*/

const directoryPath = path.join(__dirname, 'Shaders');
const args = process.argv.slice(2);

const level = args.findIndex((arg)=> arg === "-dxlevel");
const optim = args.findIndex((arg)=> arg === "-olevel");

console.log(`[INFO]: Compiling shaders...`);

fs.readdir(directoryPath, (err, files) => {
    if (err)
        return console.error(`[FATAL]: Unable to scan directory: ${err}`);

    files.forEach((file) => {
        const fileExt = file.split(".");

        if (fileExt[fileExt.length-1] === "def")
            return;

        if (fileExt[fileExt.length-1] === "frag" || fileExt[fileExt.length-1] === "vert") {
            console.info(`[INFO]: "${file}" is being compiled...`);
            compileShader(file);
            return;
        }

        console.warn(`[WARN]: "${fileExt[1]}" is not a supported format`);
    });
});

function compileShader(file) {
    const name = file.split(".");

    const input = `-f ./Shaders/${file}`;
    const output = `-o ./DX9/${file.split(".")[0]}.${name[1] == "vert" ? "bvs" : "bfs"}`;
    const type = `--type ${name[1] == "vert" ? "vertex" : "fragment"}`;
    const include = "-i ./";
    const platform = "--platform windows";
    const varying = "--varyingdef ./Shaders/Varying.def";
    const profile = "-p s_3_0"; // dx9=s_3_0 dx10=s_4_0 dx11+=s_5_0
    const optlevel = `-O ${optim != -1 ? args[optim+1] : ""}`;
    const defines = "";

    // every array item is need to be seperated at spaces...
    const array = [input, output, type, include, platform, varying, profile, optlevel].join(" ").split(" ");
    const child = child_process.spawn("./Binaries/Win32/shaderc", array);

    child.stdout.setEncoding('utf8');
    child.stdout.on('data', function (data) {
        var str = data.toString()
        var lines = str.split(/(\r?\n)/g);
        console.log(lines.join(""));
    });
}