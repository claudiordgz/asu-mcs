Try {
  $Directory = (Get-Item .).FullName
  $TmpPath = Join-Path -Path $Directory -ChildPath "tmp-bishops"
  New-Item -ItemType Directory -Force -Path $TmpPath
  3..8 | ForEach-Object { 
    $file = Join-Path -Path $TmpPath -ChildPath "stderr_n=$_.txt"
    $command = "clingo ""8.bishops.lp"" -c ""n=$_"" 0 | Out-File -FilePath $file"
    Write-Host $command
    Invoke-Expression $command
  }
} Catch {
  Write-Error "$($_.Exception.Message) - Line Number: $($_.InvocationInfo.ScriptLineNumber)"
  throw
}