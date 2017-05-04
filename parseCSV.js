var shhhCSVParser = {
    fileData: "",
    tableWrapper: '',
    resultArray: [],
    parseArray: function pa() {
        var resutlArray = [];
        var obj = this.fileData;
        obj.split("\n").forEach(function (row) {
            var rowArray = [];
            row.split(",").forEach(function (cell) {
                rowArray.push(cell);
            });
            resutlArray.push(rowArray);
        });
        this.resultArray = resutlArray;
    },
    getArray: function ga() {
        return this.resultArray;
    },
    createTable: function ct() {
        if (this.resultArray.length > 0) {
            var table = document.createElement("table");
            this.resultArray.forEach(function (row) {
                var tr = document.createElement("td");
                row.forEach(function (cell) {
                    var td = document.createElement("tr");
                    td.innerHTML = cell;
                    tr.appendChild(td);
                });
                table.appendChild(tr);
            });
            document.getElementById(this.tableWrapper).appendChild(table);
        }
    },
    init: function initilize(fileData, tableWrapper) {
        this.fileData = fileData;
        this.tableWrapper = tableWrapper;
        this.parseArray();
        this.createTable();
    }
};
