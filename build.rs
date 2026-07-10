use std::env;
use std::path::PathBuf;
use std::process::Command;

fn main() {
    let out_dir = PathBuf::from(env::var("OUT_DIR").unwrap());

    // Run bison and flex
    println!("cargo:rerun-if-changed=compiler/sigil.y");
    println!("cargo:rerun-if-changed=compiler/sigil.l");

    let status = Command::new("bison")
        .args(&["-v", "-d", "compiler/sigil.y", "-o"])
        .arg(out_dir.join("sigil.tab.c"))
        .status()
        .expect("Failed to run bison");
    if !status.success() { panic!("Bison failed"); }

    let status = Command::new("flex")
        .args(&["-o"])
        .arg(out_dir.join("lex.yy.c"))
        .arg("compiler/sigil.l")
        .status()
        .expect("Failed to run flex");
    if !status.success() { panic!("Flex failed"); }

    // Compile C code
    cc::Build::new()
        .file("compiler/ast.c")
        .file(out_dir.join("sigil.tab.c"))
        .file(out_dir.join("lex.yy.c"))
        .file("compiler/compiler_interface.c")
        .include("compiler")
        .include(&out_dir)
        .compile("sigil_compiler");

    // Generate bindings
    let bindings = bindgen::Builder::default()
        .header("compiler/compiler_interface.h")
        .header("compiler/ast.h")
        .clang_arg(format!("-I{}", out_dir.display()))
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Unable to generate bindings");

    bindings
        .write_to_file(out_dir.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
