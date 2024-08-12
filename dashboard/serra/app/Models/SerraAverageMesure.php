<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class SerraAverageMesure extends Model
{
    use HasFactory;

    protected $table = 'serra_average_mesures';

    public function serra()
    {
        return $this->belongsTo(Serra::class, 'id_serra', 'id');
    }
}
