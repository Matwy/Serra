<?php

namespace App\Http\Controllers;
use Illuminate\Http\Request;

use App\Models\User;
use App\Models\Serra;

class DashboardController extends Controller
{
    public function index()
    {
        $user_id = auth()->id();
        
        $user = User::find($user_id);
        if (!$user) {
            // Gestisci il caso in cui le serre non siano  state trovate
            return response()->json(['message' => 'Utente non trovato'], 404);
        }
        $serra = Serra::whereHas('users', function ($query) use ($user_id) {
            $query->where('users.id', $user_id);
        })->where('serra.id', 1)->first();
        
        return view('dashboard', ['serre' => $user->serre]);
    }
    public function updateName(Request $request){
        $new_name = $request->input('name');
        $serra_id = $request->input('id');
        $user_id = auth()->id();

        if(!isset($name) && !isset($serra_id) && !isset($user_id)){
            return response()->json(['error' =>'id or name not provided']);
        }

        $serra = Serra::whereHas('users', function ($query) use ($user_id) {
            $query->where('users.id', $user_id);
        })->where('serra.id', 1)->first();
        $serra->nome = $new_name;
        $serra->save();

        return response()->json(['new name' => $serra]);
    }
    public function updateImage(Request $request){
        if (!$request->hasFile('image')) {
            return response()->json(['error' =>'image not correctly provided']);
        }
        // Salva l'immagine nella cartella di destinazione
        $image = $request->file('image');
        $imageName = $image->getClientOriginalName();
        $image->move(public_path('images'), $imageName);
        return response()->json(['success' =>'image successfully updated']);
    }
}
