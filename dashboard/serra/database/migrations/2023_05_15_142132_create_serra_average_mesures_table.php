<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     */
    public function up(): void
    {
        Schema::create('serra_average_mesures', function (Blueprint $table) {
            $table->id();
            $table->unsignedBigInteger('id_serra');
            $table->decimal('temperature', 10, 2);
            $table->decimal('humidity_air', 10, 2);
            $table->decimal('pressure', 10, 2);
            $table->decimal('altitude', 10, 2);
            $table->decimal('humidity_terrain', 10, 2);
            $table->timestamps();

            $table->foreign('id_serra')->references('id')->on('serra');
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('serra_average_mesures');
    }
};
