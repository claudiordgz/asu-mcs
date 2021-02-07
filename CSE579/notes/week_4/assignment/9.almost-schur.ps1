
Try {
  $Directory = (Get-Item .).FullName
  $TmpPath = Join-Path -Path $Directory -ChildPath "tmp-schur"
  New-Item -ItemType Directory -Force -Path $TmpPath
  50..60 | ForEach-Object { 
    $file = Join-Path -Path $TmpPath -ChildPath "stderr_n=$_.txt"
    $command = "clingo ""9.almost-schur.lp"" -c k=4 -c ""n=$_"" | Out-File -FilePath $file"
    Write-Host $command

    $objProcess = New-Object System.Diagnostics.Process
    $objProcess.StartInfo = New-Object System.Diagnostics.ProcessStartInfo
    $objProcess.StartInfo.FileName = "pwsh"
    $objProcess.StartInfo.Arguments = "conda activate py3.6; clingo ""9.almost-schur.lp"" -c k=4 -c n=$_ | Out-File -FilePath $file"
    $objProcess.StartInfo.WindowStyle = 1
    $objProcess.StartInfo.RedirectStandardOutput = $true

    $null = $objProcess.Start()
    $timeout = new-timespan -Seconds 10
    $sw = [diagnostics.stopwatch]::StartNew()

    while ($sw.elapsed -lt $timeout) {
      if ($objProcess.HasExited)
      { 
        continue 
      }
      start-sleep -seconds 5
    }
  
    If ( -not ($objProcess.HasExited) ) 
    {
      Write-Host "Forcing exit"
      Stop-Process -Id $objProcess.Id -Force
      return
    }
  }
}
Catch {
  Write-Error "$($_.Exception.Message) - Line Number: $($_.InvocationInfo.ScriptLineNumber)"
  throw
}