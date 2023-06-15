open Core
open Utils

type t = {
  (* Activate debug mode *)
  debug : bool;
  (* Command to execute inside the container *)
  command : string;
  (* User id to create inside the container *)
  uid : int;
  (* Directory to mount as root of the container *)
  mount_dir : string;
}
(** Raw configuration parameters received from the command line *)

let check_mount_dir (mount_dir : string) =
    match Unix.access mount_dir [`Exists] with
    | Ok _ -> Ok mount_dir
    | Error _ -> Error (Err.Argument_invalid "mount")
