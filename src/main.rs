use clap::{Parser, Subcommand};
use serde::{Deserialize, Serialize};
use std::fs;
use std::path::PathBuf;

mod bindings {
    #![allow(non_upper_case_globals)]
    #![allow(non_camel_case_types)]
    #![allow(non_snake_case)]
    #![allow(dead_code)]
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
}

#[derive(Parser)]
#[command(name = "sigil")]
#[command(about = "Sigil v2 CLI", long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Load user state
    User {
        #[command(subcommand)]
        command: LoadCommands,
    },
    /// Load environment state
    Env {
        #[command(subcommand)]
        command: LoadCommands,
    },
    /// Load NPC state
    Npcs {
        #[command(subcommand)]
        command: LoadCommands,
    },
    /// Load inventory state
    Inventory {
        #[command(subcommand)]
        command: LoadCommands,
    },
    /// Enchant an item with a spell
    Enchant {
        #[arg(long)]
        spell: PathBuf,
        #[arg(long)]
        item: String,
    },
    /// Wield an enchanted item
    Wield {
        item: String,
    },
}

#[derive(Subcommand)]
enum LoadCommands {
    /// Load from a file
    Load {
        #[arg(long)]
        file: PathBuf,
    },
}

#[derive(Default, Serialize, Deserialize)]
struct GlobalState {
    user: Option<UserData>,
    env: Option<EnvData>,
    npcs: Vec<NpcData>,
    inventory: Vec<ItemData>,
}

#[derive(Serialize, Deserialize, Clone)]
struct UserData {
    name: String,
    skill_level: i32,
    position: [f32; 3],
}

#[derive(Serialize, Deserialize, Clone)]
struct EnvData {
    name: String,
    atmosphere: String,
}

#[derive(Serialize, Deserialize, Clone)]
struct NpcData {
    name: String,
    type_name: String,
    position: [f32; 3],
}

#[derive(Serialize, Deserialize, Clone)]
struct ItemData {
    name: String,
    type_name: String,
    enchantment: Option<String>,
    enchanted_name: Option<String>,
}

fn calculate_success_factor(skill_level: i32) -> f32 {
    // Basic success factor: skill_level % + some variance
    let variance = (rand_u32() % 20) as f32 - 10.0;
    (skill_level as f32 + variance).max(0.0) / 100.0
}

fn rand_u32() -> u32 {
    std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .unwrap()
        .as_secs() as u32
}

fn get_quality_descriptor(factor: f32) -> &'static str {
    if factor > 1.1 { "Transcendent" }
    else if factor > 0.95 { "Supreme" }
    else if factor > 0.8 { "Resonant" }
    else if factor > 0.5 { "Stable" }
    else { "Faint" }
}

fn get_essence_descriptor(spell_path: &PathBuf) -> &'static str {
    let path_str = spell_path.to_str().unwrap_or("");
    if path_str.contains("fireball") || path_str.contains("heat") { "Flaming" }
    else if path_str.contains("force") || path_str.contains("push") { "Thundering" }
    else { "Mystical" }
}

fn get_random_suffix() -> &'static str {
    let suffixes = ["Retribution", "Fury", "Grace", "Judgment", "Legacy"];
    suffixes[(rand_u32() as usize) % suffixes.len()]
}

fn main() {
    let cli = Cli::parse();
    let mut state = load_state();

    match cli.command {
        Commands::User { command } => match command {
            LoadCommands::Load { file } => {
                let content = fs::read_to_string(file).expect("Failed to read file");
                let user: UserData = serde_yaml::from_str(&content).expect("Failed to parse YAML");
                println!("Welcome, {}!", user.name);
                state.user = Some(user);
                save_state(&state);
            }
        },
        Commands::Env { command } => match command {
            LoadCommands::Load { file } => {
                let content = fs::read_to_string(file).expect("Failed to read file");
                let env: EnvData = serde_yaml::from_str(&content).expect("Failed to parse YAML");
                println!("Environment \"{}\" loaded.", env.name);
                state.env = Some(env);
                save_state(&state);
            }
        },
        Commands::Npcs { command } => match command {
            LoadCommands::Load { file } => {
                let content = fs::read_to_string(file).expect("Failed to read file");
                let npcs: Vec<NpcData> = serde_yaml::from_str(&content).expect("Failed to parse YAML");
                println!("NPC group loaded.");
                state.npcs = npcs;
                save_state(&state);
            }
        },
        Commands::Inventory { command } => match command {
            LoadCommands::Load { file } => {
                let content = fs::read_to_string(file).expect("Failed to read file");
                let items: Vec<ItemData> = serde_yaml::from_str(&content).expect("Failed to parse YAML");
                for item in &items {
                    println!("{} \"{}\" equipped!", item.type_name, item.name);
                }
                state.inventory = items;
                save_state(&state);
            }
        },
        Commands::Enchant { spell, item } => {
            let skill = state.user.as_ref().map(|u| u.skill_level).unwrap_or(0);
            println!("Compiling spell {} with skill level {}...", spell.display(), skill);
            
            let c_spell = std::ffi::CString::new(spell.to_str().unwrap()).unwrap();
            unsafe {
                let root = bindings::parse_sigil_file(c_spell.as_ptr());
                if root.is_null() {
                    eprintln!("Failed to parse spell.");
                    std::process::exit(1);
                }
                
                let factor = calculate_success_factor(skill);
                let quality = get_quality_descriptor(factor);
                let essence = get_essence_descriptor(&spell);
                let suffix = get_random_suffix();
                
                if let Some(i) = state.inventory.iter_mut().find(|i| i.name == item) {
                    let enchanted_name = format!("{} {} {} of {}", quality, essence, i.name, suffix);
                    i.enchantment = Some(spell.to_str().unwrap().to_string());
                    i.enchanted_name = Some(enchanted_name.clone());
                    
                    println!("\nYour mastery of the Field is unmatched! You have produced a perfect enchantment!");
                    println!("{} item \"{}\" updated to \"{}\"!", i.type_name, i.name, enchanted_name);
                } else {
                    println!("Item \"{}\" not found in inventory.", item);
                }
                bindings::free_ast(root);
            }
            save_state(&state);
        }
        Commands::Wield { item } => {
            if let Some(i) = state.inventory.iter().find(|i| {
                i.name == item || 
                i.enchanted_name.as_ref().map(|en| en.contains(&item)).unwrap_or(false)
            }) {
                let display_name = i.enchanted_name.as_ref().unwrap_or(&i.name);
                if let Some(spell_path) = &i.enchantment {
                    println!("Activating \"{}\" (source: {})...", display_name, spell_path);
                    
                    // Distance-based targeting
                    if let Some(user) = &state.user {
                        let mut nearest_npc_idx = None;
                        let mut min_dist = f32::MAX;
                        
                        for (idx, npc) in state.npcs.iter().enumerate() {
                            let dist = ((npc.position[0] - user.position[0]).powi(2) +
                                       (npc.position[1] - user.position[1]).powi(2) +
                                       (npc.position[2] - user.position[2]).powi(2)).sqrt();
                            if dist < min_dist {
                                min_dist = dist;
                                nearest_npc_idx = Some(idx);
                            }
                        }
                        
                        // Let's say effective radius is 10.0
                        if let Some(idx) = nearest_npc_idx {
                            if min_dist <= 10.0 {
                                let npc_name = state.npcs[idx].name.clone();
                                println!("\n{} was targeted (Distance: {:.2})!", npc_name, min_dist);
                                println!("A sphere of intense heat rockets toward the target!");
                                println!("The target was incinerated!");
                                state.npcs.remove(idx);
                                save_state(&state);
                            } else {
                                println!("\nNo enemies within range (Nearest: {} at {:.2})", state.npcs[idx].name, min_dist);
                            }
                        } else {
                            println!("\nNo targets found in the environment.");
                        }
                    } else {
                        println!("User state not loaded.");
                    }
                } else {
                    println!("Item \"{}\" is not enchanted.", i.name);
                }
            } else {
                println!("Item \"{}\" not found.", item);
            }
        }
    }
}

fn load_state() -> GlobalState {
    let path = PathBuf::from("sigil_state.yml");
    if path.exists() {
        let content = fs::read_to_string(path).unwrap_or_default();
        serde_yaml::from_str(&content).unwrap_or_default()
    } else {
        GlobalState::default()
    }
}

fn save_state(state: &GlobalState) {
    let content = serde_yaml::to_string(state).expect("Failed to serialize state");
    fs::write("sigil_state.yml", content).expect("Failed to write state file");
}
