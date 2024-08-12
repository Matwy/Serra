<x-app-layout>
    <div id="graphs-container" data-serre="{{ json_encode($serre) }}"></div>

    <div class="relative items-center justify-center rounded-lg border-4 border-green-500 p-4 mt-16 mx-8 shadow-xl">
        <div class="flex justify-center m-4">
            <div class="flex items-center py-2">
                <cite id="editableCite"
                    class="appearance-none bg-transparent border-none w-full text-gray-800 mr-3 py-1 px-2 not-italic font-semibold text-lg focus:outline-none"
                    contenteditable="false">{{ $serre[0]->nome }} </cite>
            </div>
            <button id="editButton" onclick="toggleEdit()"
                class="flex-shrink-0 bg-green-500 hover:bg-green-700 text-white font-bold py-2 px-4 rounded-full">Edit</button>
        </div>

        <div class="flex justify-center gap-16 my-2">
            <div class="relative inline-block items-center content-center">
                <img src="https://www.piccoli-elettrodomestici.com/wp-content/uploads/2018/11/Migliore-Serra-Idroponica.jpg"
                    alt="Serra" class=" w-256 h-96 rounded-3xl">
                <!--<div onclick="document.getElementById('file-input').click();"
                    class="absolute left-2 top-2 bg-green-500 hover:bg-green-700 text-white font-bold py-2 px-4 rounded-full">
                    <img width="16" height="16" src="https://img.icons8.com/material-rounded/48/upload--v1.png"
                        alt="upload--v1" />
                    <input id="file-input" type="file" class="hidden">
                </div>-->

            </div>
            <div class="grid grid-cols-2 gap-4 content-center">
                <div>
                    <div class="relative mx-auto h-60 w-96">
                        <canvas id="temperature-chart" class="chart-canvas"></canvas>
                    </div>
                    <div class="relative mx-auto h-60 w-96">
                        <canvas id="humiditys-chart" class="chart-canvas"></canvas>
                    </div>
                </div>
                <div>
                    <div class="relative mx-auto h-60 w-96">
                        <canvas id="pressure-chart" class="chart-canvas"></canvas>
                    </div>
                    <div class="relative mx-auto h-60 w-96">
                        <canvas id="altitude-chart" class="chart-canvas"></canvas>
                    </div>
                </div>
            </div>
        </div>
        <div class="flex justify-around mx-8 mb-4">
            <div id="water-button"
                class="flex rounded-full border-4 border-[#FF0000] bg-white text-center inset-0 flex items-center justify-center p-3 focus:border-blue-500">
                <img width="32" height="32" src="https://img.icons8.com/ios-glyphs/60/temperature--v2.png"
                    alt="temperature--v2" />
                <span class="text-gray-700 text-xl font-bold">water the greenhouse</span>
            </div>
        </div>
        <div class="flex justify-around mx-8">
            <div
                class="flex rounded-full border-4 border-[#FF0000] bg-white text-center inset-0 flex items-center justify-center p-3">
                <img width="32" height="32" src="https://img.icons8.com/ios-glyphs/60/temperature--v2.png"
                    alt="temperature--v2" />
                <span id="live-value-1" class="text-gray-700 text-xl font-bold">0 °C</span>
            </div>
            <div
                class="flex rounded-full border-4 border-[#994F00] bg-white text-center inset-0 flex items-center justify-center p-3">
                <img width="32" height="32" src="https://img.icons8.com/metro/52/humidity.png" alt="humidity" />
                <span id="live-value-2" class="text-gray-700 text-xl font-bold">0 %</span>
            </div>
            <div
                class="flex rounded-full border-4 border-[#8FFFDE] bg-white text-center inset-0 flex items-center justify-center p-3">
                <img width="32" height="32" src="https://img.icons8.com/ios-filled/50/air-element.png"
                    alt="air-element" />
                <span id="live-value-3" class="text-gray-700 text-xl font-bold">0 %</span>
            </div>
            <div
                class="flex rounded-full border-4 border-[#0000FF] bg-white text-center inset-0 flex items-center justify-center p-3">
                <img width="32" height="32" src="https://img.icons8.com/ios-glyphs/60/pressure.png"
                    alt="pressure" />
                <span id="live-value-4" class="text-gray-700 text-xl font-bold">0 hPa</span>
            </div>
            <div
                class="flex rounded-full border-4 border-[#FFA500] bg-white text-center inset-0 flex items-center justify-center p-3">
                <img width="32" height="32"
                    src="https://img.icons8.com/external-outline-black-m-oki-orlando/32/external-altitude-geography-outline-outline-black-m-oki-orlando.png"
                    alt="external-altitude-geography-outline-outline-black-m-oki-orlando" />
                <span id="live-value-5" class="text-gray-700 text-xl font-bold">0 m</span>
            </div>
        </div>
    </div>

    <script>
        function toggleEdit() {
            const editButton = document.getElementById('editButton');
            const editableCite = document.getElementById('editableCite');

            if (editButton.textContent === 'Edit') {
                editButton.textContent = 'Save';
                editableCite.contentEditable = 'true';
                return
            }
            editButton.textContent = 'Edit';
            editableCite.contentEditable = 'false';

            // Salvare il valore modificato qui o eseguire altre azioni necessarie
            const modifiedValueJSON = JSON.stringify({
                name: editableCite.textContent,
                id_serra: "{{ $serre[0]->id }}"
            });

            fetch('/dashboard/update-name', {
                method: 'POST',
                headers: {
                    'X-CSRF-TOKEN': document.querySelector('meta[name="csrf-token"]').getAttribute('content'),
                    'Content-Type': 'application/json'
                },
                body: modifiedValueJSON
            })
        }
    </script>

    @vite(['resources/js/graphs.js'])
</x-app-layout>
